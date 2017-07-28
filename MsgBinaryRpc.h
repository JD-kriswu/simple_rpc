#ifndef _MSGBINARYRPC_H_
#define _MSGBINARYRPC_H_
#include"MsgRpc.h"



using namespace std;
using namespace tencent::pbrpc;
class MsgBinaryRpc : public MsgRpc 
{
    public:
        MsgBinaryRpc():MsgRpc()
        {

        }
        virtual ~MsgBinaryRpc()
        {

        }
        virtual void HandleError();
        int AssembleSucceedResponse(
                RpcController* cntl,
                const google::protobuf::Message* response,
                std::string& err,std::string &out);
        int DecodeRequest(char *sBuf,int iLen);
        void CallMethod(
                MethodBoard* method_board,
                RpcController* controller,
                google::protobuf::Message* request,
                google::protobuf::Message* response);
        void OnCallMethodDone(
                MethodBoard* method_board,
                RpcController* controller,
                google::protobuf::Message* request,
                google::protobuf::Message* response);



};
#endif

