#include"MsgBinaryRpc.h"
#include "syncincl.h"
#include<map>
#include<algorithm>
#include"rpc_message_header.h"
#include "rpc_meta.pb.h"
using namespace tencent::pbrpc;
using namespace std;

void MsgBinaryRpc::HandleError()
{
    return;
}

int MsgBinaryRpc::AssembleSucceedResponse(
		RpcController* cntl,
		const google::protobuf::Message* response,
		std::string& err,std::string &out)
{
	RpcMeta meta;
	meta.set_type(RpcMeta::RESPONSE);
	meta.set_sequence_id(cntl->SequenceId());
	meta.set_failed(false);
	meta.set_compress_type(cntl->ResponseCompressType());
	meta.set_method(cntl->MethodId());

	RpcMessageHeader header;
	int header_size = static_cast<int>(sizeof(header));
	header.meta_size = htonl(meta.ByteSize());
	header.data_size = htonll(response->ByteSize());

	out.resize(header_size+meta.ByteSize()+response->ByteSize());

	char *pCur=(char *)out.data();
	memcpy(pCur,(char *)&header,header_size);
	pCur+=header_size;
	if (!meta.SerializeToArray(pCur,meta.ByteSize()))
	{
		GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"serialize rpc meta failed\n");
		return -1 ;
	}

	pCur+=meta.ByteSize();

	bool ser_ret = false;
	if (meta.compress_type() == CompressTypeNone)
	{
		ser_ret = response->SerializeToArray(pCur,response->ByteSize());
	}
	else
	{
		GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"not support compress response\n");

	}
	if (!ser_ret)
	{
		GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"serialize response message failed\n");
		return -2 ;
	}


	return 0;
}

void MsgBinaryRpc::CallMethod(
		MethodBoard* method_board,
		RpcController* controller,
		google::protobuf::Message* request,
		google::protobuf::Message* response)
{
	method_board->ReportProcessBegin();
	google::protobuf::Closure* done = NewClosure(
			this, &MsgRpc::OnCallMethodDone,
			method_board, controller, request, response);
	method_board->GetServiceBoard()->Service()->CallMethod(
			method_board->Descriptor(), controller, request, response, NULL);
   done->Run();
}

void MsgBinaryRpc::OnCallMethodDone(
		MethodBoard* method_board,
		RpcController* controller,
		google::protobuf::Message* request,
		google::protobuf::Message* response)
{
	string err,strRsp;
	//cntl->SetFinishProcessTime(ptime_now());
	int64 process_time_us=0;//   (cntl->FinishProcessTime() - cntl->StartProcessTime()).total_microseconds();
	if (controller->Failed())
	{
		method_board->ReportProcessEnd(false, process_time_us);
		blob_type blob;
		blob.data = (char*)NULL;
		blob.len  = (int)0;    
		this->SendToClient(blob);
	}
	else
	{
		blob_type blob;

		method_board->ReportProcessEnd(true, process_time_us);
		if(AssembleSucceedResponse(controller,response,err,strRsp)<0)
		{
			blob.data = (char*)NULL;
			blob.len  = 0;
		}
		else
		{
			blob.data = (char*)strRsp.data();
			blob.len  = (int)strRsp.size();    
		}
		this->SendToClient(blob);
	}

}

int MsgBinaryRpc::DecodeRequest(char *sBuf,int iLen)
{
	std::string service_name;
	std::string method_name;  
	std::string method_full_name;
	CompressType req_compress_type=CompressTypeNone;
	CompressType rsp_compress_type=CompressTypeNone;
	int64 qSeqId=1;
	int64   data_size=0; //real data size
	char *pCur=sBuf;
	RpcMeta meta;
	RpcMessageHeader *pHeader;
	pHeader=(RpcMessageHeader*)pCur;
	if(!pHeader->CheckMagicString() )
	{
		GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"magic wrong\n");
        HandleError();
		return -1;
	}
	int32   meta_size=ntohl(pHeader->meta_size);    // 4 bytes
	data_size=ntohll(pHeader->data_size); //8 bytes  

	if(iLen!=sizeof(RpcMessageHeader)+meta_size+data_size)
	{
		GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"binary request size wrong\n");
        HandleError();
		return -1;
	}
	pCur += sizeof(RpcMessageHeader);
	if(!meta.ParseFromArray(pCur, meta_size) ) return -1;
	pCur+=meta_size;
	method_full_name=meta.method();
	rsp_compress_type=meta.has_expected_response_compress_type() ?meta.expected_response_compress_type() : CompressTypeNone;
	req_compress_type=meta.has_compress_type()?meta.compress_type(): CompressTypeNone;
	qSeqId=meta.sequence_id();
	//printf("request method:%s\n",method_full_name.c_str());
	if(!ParseMethodFullName(method_full_name,&service_name,&method_name))
	{

		GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"ParseMethodFullName wrong:%s\n",method_full_name.c_str());
        HandleError();
		return -1;
	}

	MethodBoard* method_board = FindMethodBoard(&g_svr_pool, service_name, method_name);
	if (method_board == NULL)
	{
		GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"find no method_borad:%s\n",method_full_name.c_str());
        HandleError();
		return -1;
	}

	google::protobuf::Service* service = method_board->GetServiceBoard()->Service();
	const google::protobuf::MethodDescriptor* method_desc = method_board->Descriptor();

	GetServerBase()->log_.LOG_P_PID(LOG_DEBUG,"get methodDesc:%s\n",method_desc->DebugString().c_str());

	google::protobuf::Message* request = service->GetRequestPrototype(method_desc).New();

	//printf("new a request\n");


	if(!request->ParseFromArray(pCur,data_size))
	{
		GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"parse request failed\n");
        HandleError();
		return -1;
	}

	//printf("new a response\n");

	google::protobuf::Message* response = service->GetResponsePrototype(method_desc).New(); 
	//printf("new a response succeed\n");
	//ok ,we got a request

	//new a controler
	RpcController* controller = new RpcController();
	controller->SetSequenceId(qSeqId);
	controller->SetMethodId(method_full_name);
	controller->SetRequestCompressType(req_compress_type);
	controller->SetResponseCompressType(rsp_compress_type); 
    controller->SetMsg(this);


	setRequest(request);
	setResponse(response);
	setController(controller);
	setMethodBoard(method_board);
	return 0;
}
