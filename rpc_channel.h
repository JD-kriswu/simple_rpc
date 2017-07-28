// Author:  (norvallu)

#ifndef _TENCENT_PBRPC_RPC_CHANNEL_H_
#define _TENCENT_PBRPC_RPC_CHANNEL_H_

#include <vector>

#include <google/protobuf/service.h>

#include <netdb.h>
#include "common.h"

namespace tencent {
    namespace pbrpc {

        class RpcChannel : public google::protobuf::RpcChannel
        {


            public:
                // Create single server point by server address.
                // The "rpc_client" is owned by the caller.
                // The "server_address" should be in format of "ip:port".
                RpcChannel();
                RpcChannel(const std::string& server_address  );
                RpcChannel( const std::string ip,int port);
                RpcChannel( struct sockaddr_in &stAddr);
                // Destructor.
                virtual ~RpcChannel();


                // Implements the google::protobuf::RpcChannel interface.  If the
                // "done" is NULL, it's a synchronous call, or it's asynchronous and
                // uses the callback to inform the completion (or failure). 
                virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                        ::google::protobuf::RpcController* controller,
                        const ::google::protobuf::Message* request,
                        ::google::protobuf::Message* response,
                        ::google::protobuf::Closure* done)=0;

                void SetAddr( const std::string ip,int port); 
                void SetAddr(std::string server_address);
                struct in_addr *atoaddr(char *address)
                {
                    struct hostent *host;
                    static struct in_addr saddr;

                    /* First try it as aaa.bbb.ccc.ddd. */
                    saddr.s_addr = inet_addr(address);
                    if (saddr.s_addr != -1) {
                        return &saddr;
                    }
                    host = gethostbyname(address);
                    if (host != NULL) {
                        return (struct in_addr *) *host->h_addr_list;
                    }
                    return NULL;
                }

                int MakeSockAddrIn(struct sockaddr_in *pstAddr, const char *sIP, const char *sPort)
                {
                    int iPort;
                    struct in_addr *pInAddr;

                    memset(pstAddr, 0, sizeof(struct sockaddr_in));
                    if ((pInAddr = atoaddr((char *)sIP)) != NULL && 
                            (iPort = htons(atoi(sPort))) > 0) 
                    {
                        pstAddr->sin_family = AF_INET;
                        pstAddr->sin_addr.s_addr = pInAddr->s_addr;
                        pstAddr->sin_port = iPort;
                        return 0;
                    }

                    return -1;
                }


                bool ResolveAddress( const std::string& address,struct sockaddr_in &stAddr)
                {
                    std::string::size_type pos = address.find(':');
                    if (pos == std::string::npos)
                    {
                        return false;
                    }
                    std::string host = address.substr(0, pos);
                    std::string svc = address.substr(pos + 1);
                    return  MakeSockAddrIn(&stAddr,host.c_str(), svc.c_str())==0;
                }
            public:
                std::string _server_address;
                struct sockaddr_in _stAddr;


                TENCENT_PBRPC_DISALLOW_EVIL_CONSTRUCTORS(RpcChannel);
        }; // class RpcChannel

    } // namespace pbrpc
} // namespace tencent

#endif // _TENCENT_PBRPC_RPC_CHANNEL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
