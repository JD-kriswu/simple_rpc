// Author:  (norvallu)

#ifndef _TENCENT_PBRPC_SIMPLE_RPC_CHANNEL_IMPL_H_
#define _TENCENT_PBRPC_SIMPLE_RPC_CHANNEL_IMPL_H_
#include<stdio.h>
#include <rpc_channel.h>
#include <rpc_controller.h>
#include<string>
#include<stdint.h>


namespace tencent {
	namespace pbrpc {

		class SimpleRpcChannelImpl : public RpcChannel
		{
			public:
				SimpleRpcChannelImpl();
				SimpleRpcChannelImpl( const std::string& server_address);
                SimpleRpcChannelImpl( const std::string ip,int port);
                SimpleRpcChannelImpl( struct sockaddr_in &stAddr);

				virtual ~SimpleRpcChannelImpl();

				virtual void Stop();

				virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
						::google::protobuf::RpcController* controller,
						const ::google::protobuf::Message* request,
						::google::protobuf::Message* response,
						::google::protobuf::Closure* done);
                static int HandleInput( void *sBuf, int iLen);
				static int  PackRequest( uint64_t seqid, std::string  method, const ::google::protobuf::Message* request, std::string & out);
				static int  UnPackRsponse( char *sRecv, int iLen, ::google::protobuf::Message& response);


				TENCENT_PBRPC_DISALLOW_EVIL_CONSTRUCTORS(SimpleRpcChannelImpl);
		}; // class SimpleRpcChannelImpl

	} // namespace pbrpc
} // namespace tencent

#endif // _TENCENT_PBRPC_SIMPLE_RPC_CHANNEL_IMPL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
