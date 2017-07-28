// Author:  (norvallu)

#include <simple_rpc_channel_impl.h>
#include <rpc_error_code.h>
#include"rpc_message_header.h"
#include "rpc_meta.pb.h"
#include <closure.h>
#include<string>
#include"syncincl.h"

using namespace std;
namespace tencent {
	namespace pbrpc {

		int SimpleRpcChannelImpl::HandleInput( void *sBuf, int iLen)
		{
			using namespace tencent::pbrpc;
			char *pCur=(char *)sBuf;
			if(iLen<sizeof(RpcMessageHeader)) return 0;
			RpcMessageHeader *pHeader=(RpcMessageHeader *)sBuf;

			return sizeof(RpcMessageHeader) + ntohl(pHeader->meta_size) + ntohll(pHeader->data_size);

		}
		SimpleRpcChannelImpl::SimpleRpcChannelImpl( ):RpcChannel()
		{

		}
		SimpleRpcChannelImpl::SimpleRpcChannelImpl(const std::string& server_address ):RpcChannel(server_address)
		{
		}
        SimpleRpcChannelImpl::SimpleRpcChannelImpl( const std::string ip,int port):RpcChannel(ip,port)
        {

        }

        SimpleRpcChannelImpl::SimpleRpcChannelImpl( struct sockaddr_in &stAddr):RpcChannel(stAddr)
        {
        }

		SimpleRpcChannelImpl::~SimpleRpcChannelImpl()
		{
		}


		void SimpleRpcChannelImpl::Stop()
		{
		}

		void SimpleRpcChannelImpl::CallMethod(const ::google::protobuf::MethodDescriptor* method,
				::google::protobuf::RpcController* controller,
				const ::google::protobuf::Message* request,
				::google::protobuf::Message* response,
				::google::protobuf::Closure* done)
		{
			// prepare controller
			RpcController* cntl = dynamic_cast<RpcController*>(controller);
			cntl->FillFromMethodDescriptor(method);
			//pack msg
			string sOut;
			if(PackRequest(cntl->SequenceId(),cntl->MethodId(),request,sOut)<0)
			{
				printf("pack request failed\n");
				cntl->SetFailed(RPC_ERROR_PARSE_REQUEST_MESSAGE, "packe request failed\n");
				return;
			}
			//mt_send_and_recv
			char *pRecvBuf;
		
			char sRecv[1024*64]; //todo, it is not flexible
			int iRecvSize;
			int iRet=0;
			cntl->GetRecvBuf(&pRecvBuf,&iRecvSize);
			if(pRecvBuf==NULL)
			{
				pRecvBuf=sRecv;
				iRecvSize=sizeof(sRecv);
			}
			if(cntl->GetServiceType()=="UDP")
			{
				iRet=mt_udpsendrcv(&_stAddr, (char *)sOut.data(), sOut.size(), pRecvBuf, iRecvSize, cntl->Timeout());
			}
			else
			{
				iRet=mt_tcpsendrcv(&_stAddr,(char *)sOut.data(), sOut.size() , pRecvBuf,iRecvSize, cntl->Timeout(), HandleInput);

			}

			if(iRet<0)
			{
				int errcode=-1;
				string sErrMsg="";
				if(iRet==-6)
					{
					errcode=RPC_ERROR_RECV_BUFFER_OVERFLOW;
					sErrMsg="recv buf too small";
					}
				else if(iRet==-5)
					{
					errcode=RPC_ERROR_RECV_CHECK_COMPLETE;
					sErrMsg="error check recv complete";
					}
				else if(iRet==-7)
					{
					errcode=RPC_ERROR_PEER_CLOSED;	
					sErrMsg="tcp peer closed";
					}
				else if(iRet==-2)
					{
					errcode=RPC_ERROR_SEND;	
					sErrMsg="send failed";
					}
				else if(iRet==-4)
					{
					errcode=RPC_ERROR_CONNECTION_CLOSED;	
					sErrMsg="connection failed";
					}
				else if(iRet==-1)
					{
					errcode=RPC_ERROR_SYS;	
					sErrMsg="system error";
					}
				else if(iRet==-3)
					{
					errcode=RPC_ERROR_REQUEST_TIMEOUT;	
					sErrMsg="timeout or recv failed from remotel";
					}
		
		
				cntl->SetFailed(errcode, sErrMsg);
			}
			else
			{
				//unpack msg
				if((iRet=UnPackRsponse(pRecvBuf,iRecvSize,*response))<0)
				{
                    //printf("unpack error:%d\n",iRet);
					cntl->SetFailed(RPC_ERROR_PARSE_RESPONSE_MESSAGE, "parse response failed\n");
					return;
				}
			}
			return ;

		}


		int  SimpleRpcChannelImpl::PackRequest( uint64_t seqid, string  method,const ::google::protobuf::Message* request, string & out)
		{
			char *pCur;
			RpcMessageHeader header;
			RpcMeta meta;
			meta.set_type(RpcMeta::REQUEST);
			meta.set_sequence_id(seqid);
			meta.set_method(method);
			meta.set_compress_type(CompressTypeNone);
			meta.set_expected_response_compress_type(CompressTypeNone);	 
			out.resize(sizeof(RpcMessageHeader)+meta.ByteSize()+request->ByteSize());

			header.meta_size=htonl(meta.ByteSize());
			header.data_size=htonll(request->ByteSize());	
			pCur=(char *)out.data();
			memcpy(pCur,(char *)&header,sizeof(header));
			pCur+=sizeof(header);
			if (!meta.SerializeToArray(pCur,meta.ByteSize()))
			{
				return -1 ;
			}

			bool ser_ret = false;
			pCur+=meta.ByteSize();
			//printf("request compress type:%d\n",int(meta.compress_type()));
			if (meta.compress_type() == CompressTypeNone)
			{
				ser_ret = request->SerializeToArray(pCur,request->ByteSize());
				//printf("serial request:%d\n",ser_ret);
			}
			else 
			{

				//printf("we not support compress now\n");
				return -2;

			}
			return 0;
		}


		int  SimpleRpcChannelImpl::UnPackRsponse( char *sRecv, int iLen, ::google::protobuf::Message& response)
		{
			char *pCur=sRecv;
			RpcMeta meta;
			RpcMessageHeader *pHeader;
			pHeader=(RpcMessageHeader*)pCur;
			if(!pHeader->CheckMagicString() )
			{
				return -1;
			}
			int32   meta_size=ntohl(pHeader->meta_size);    // 4 bytes
			int64   data_size=ntohll(pHeader->data_size); //8 bytes  
			if(iLen!=sizeof(RpcMessageHeader)+meta_size+data_size)
			{
				return -2;
			}
			pCur += sizeof(RpcMessageHeader);
			if(!meta.ParseFromArray(pCur, meta_size) ) return -3;
			pCur+=meta_size;
			if(!response.ParseFromArray(pCur,data_size)) return -4;
			return 0;
		}


	} // namespace pbrpc
} // namespace tencent

/* vim: set ts=4 sw=4 sts=4 tw=100 */
