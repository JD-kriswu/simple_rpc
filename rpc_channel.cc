// Author:  (norvallu)

#include <sstream>
#include "rpc_channel.h"
#include <simple_rpc_channel_impl.h>
#include<utils2.h>
#include<string>
using namespace std;

namespace tencent {
	namespace pbrpc {


		RpcChannel::RpcChannel()
		{
		}
		RpcChannel::RpcChannel( const std::string& server_address)
		{
			SetAddr(server_address);
		}
		RpcChannel::RpcChannel( const std::string ip,int port)
		{

			std::string server_address=ip+":"+tce::ToStr(port);
			SetAddr(server_address);
		}

		RpcChannel::RpcChannel( struct sockaddr_in &stAddr)
		{
			std::string server_address=string(inet_ntoa(stAddr.sin_addr))+":" + tce::ToStr(ntohs(stAddr.sin_port));
			SetAddr(server_address);
		}
		void RpcChannel::SetAddr( const std::string ip,int port)
		{
			std::string server_address=ip+":"+tce::ToStr(port);
			SetAddr(server_address);
		}

		RpcChannel::~RpcChannel()
		{
		}
        void RpcChannel::SetAddr(std::string server_address)
        {
            _server_address=server_address;
            ResolveAddress(_server_address,_stAddr);
        }


	} // namespace pbrpc
} // namespace tencent

/* vim: set ts=4 sw=4 sts=4 tw=100 */
