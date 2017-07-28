#include <sstream>
#include <stdarg.h>
#include <pbjson.h>
#include "utils.h"
#include "MsgHttpRpc.h"
#include "rpc_message_header.h"
#include "rpc_meta.pb.h"

using namespace tencent::pbrpc;
using namespace std;
using namespace hconn;	



bool ParseJson(const char* str, std::string& err, rapidjson::Document & d)
{
    d.Parse<0>(str);
    if (d.HasParseError())
    {
        err = d.GetParseError();
        return false;
    }
    return true;
}


void MsgHttpRpc::HandleError()
{

    string  sBuf="";
    string errmsg="{\"code\":-113,\"message\":\"illegal request\"}";
    sBuf ="HTTP/1.1 406 Not Acceptable\r\n";
    sBuf+="Server: spprpc\r\n";
    sBuf+="Connection: closed\r\n";
    sBuf+="Content-Type: text/html\r\n";
    sBuf+="Content-Length: ";
    sBuf+=hconn::ToStr(errmsg.size());
    sBuf+="\r\n\r\n";
    sBuf+=errmsg;
    blob_type blob;
    blob.data = (char*)sBuf.data();
    blob.len  = (int)sBuf.size();    
    this->SendToClient(blob);


}

void MsgHttpRpc::CallMethod(
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

void MsgHttpRpc::OnCallMethodDone(
        MethodBoard* method_board,
        RpcController* controller,
        google::protobuf::Message* request,
        google::protobuf::Message* response)
{
    string err;
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
        string  sBuf="";
        method_board->ReportProcessEnd(true, process_time_us);
        int iBodySize=0;

        if(m_strOutPut.size()>0)
        {
            iBodySize=m_strOutPut.size();
            sBuf.append("HTTP/1.1 ");
            sBuf=sBuf+m_RespCode+"\r\n";
            sBuf.append("Server: pbrpc-server\r\n");
            sBuf.append("Content-Type: text/html\r\n");
            sBuf.append("Access-Control-Allow-Origin: *\r\n");
        }
        else
        {
            GetServerBase()->log_.LOG_P_PID(LOG_DEBUG,"before pb2json:%s\n",response->DebugString().c_str());
            pb2json(response, m_strOutPut);
            iBodySize=m_strOutPut.size();
            sBuf.append("HTTP/1.1 ");
            sBuf=sBuf+m_RespCode+"\r\n";
            sBuf.append("Server: pbrpc-server\r\n");
            sBuf.append("Content-Type: application/json\r\n");
            sBuf.append("Access-Control-Allow-Origin: *\r\n");
        }
        map<string,string>::iterator it;
        for(it=_respons_head.begin();it!=_respons_head.end();it++)
        {
            sBuf.append(it->first+": "+it->second+"\r\n");
        }

        sBuf.append("Content-Length: ");
        sBuf.append(hconn::ToStr(iBodySize));
        sBuf.append("\r\n\r\n");

        sBuf+=m_strOutPut;
        //GetServerBase()->log_.LOG_P_PID(LOG_DEBUG,"send back Body::%s\n",m_strOutPut.c_str());

        blob_type blob;
        blob.data = (char*)sBuf.data();
        blob.len  = (int)sBuf.size();    
        this->SendToClient(blob);
        GetServerBase()->log_.LOG_P_PID(LOG_DEBUG,"send back:%s\n",blob.data);
    }

}



int MsgHttpRpc::DecodeRequest(char *sBuf,int iLen)
{
    std::string service_name;
    std::string method_name;  
    std::string method_full_name;
    CompressType req_compress_type=CompressTypeNone;
    CompressType rsp_compress_type=CompressTypeNone;
    int64 qSeqId=1;
    int64   data_size=0; //real data size

    if(!_httpParse.Decode(sBuf, iLen,1))
    {
        HandleError();
        return -1;
    }
    int iType=_httpParse.GetMethod();

    string _decoded_path=_httpParse.GetURI();  //uri
    string query_string=_httpParse.GetHead(HT_URI_VALUES);
    method_full_name=_httpParse.GetHead(HT_URI);
    if(method_full_name.size()>0)
    {
        method_full_name=method_full_name.substr(1,method_full_name.size()-1);
    }
    
    GetServerBase()->log_.LOG_P_PID(LOG_DEBUG,"request method:%s\n",method_full_name.c_str());
    if(!ParseMethodFullName(method_full_name,&service_name,&method_name))
    {

        GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"ParseMethodFullName %s wrong\n",method_full_name.c_str());
        HandleError();
        return -1;
    }

    string sContentType=_httpParse.GetHead(HT_CONTENT_TYPE);
    
    if (iType==METHOD_GET)
    {
        m_sContentBody = GetQuery("request");
        GetServerBase()->log_.LOG_P_PID(LOG_DEBUG,"get request:%s\n",query_string.c_str());
    }
    else
    {
        m_sContentBody =_httpParse.GetHead(HT_BODY);
        int ContentLen=_httpParse.GetContentLength();
        m_sContentBody.assign(_httpParse.GetHead(HT_BODY),ContentLen);

    }

    if (m_sContentBody.empty())
    {
        // if null json str, set as null object
        m_sContentBody = "{}";
        GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"json str is NULL\n");
    }
    else 
    {
        GetServerBase()->log_.LOG_P_PID(LOG_DEBUG,"json str:%s\n",m_sContentBody.c_str());

    }


    std::string err;
    rapidjson::Document  _req_json;
    bool b; 
    if(strncasecmp("application/json",sContentType.c_str(),sContentType.size())==0)
    {
         b= ParseJson(m_sContentBody.c_str(), err,_req_json);
    }
    else
    {
        b= ParseJson("{}", err,_req_json);
    }

    if (!b)
    {
        GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"Parse json failed:%s\n",m_sContentBody.c_str());
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

    if (jsonobject2pb(&_req_json, request, err) < 0)
    {
        GetServerBase()->log_.LOG_P_PID(LOG_ERROR,"json2pb failed:%s\n",m_sContentBody.c_str());
        HandleError();
        delete request;
        return -2;
    }


    google::protobuf::Message* response = service->GetResponsePrototype(method_desc).New(); 
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
