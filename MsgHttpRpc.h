#ifndef _MSGHTTPRPC_H_
#define _MSGHTTPRPC_H_
#include"MsgRpc.h"
#include "http_parser.h"
#include"rapidjson/document.h"
using namespace hconn;
using namespace std;
using namespace tencent::pbrpc;

class MsgHttpRpc : public MsgRpc 
{
    public:
        MsgHttpRpc():MsgRpc()
        {
            m_RespCode="200 OK";
        }
        virtual ~MsgHttpRpc()
        {
        }
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
        virtual void HandleError();

        string GetQuery(string key)
        {
            return _httpParse.GetValue(key);
            //         return _query_params[key];
        }

        void  SetHttpRespHead(std::string key,std::string value)
        {
            _respons_head.insert(make_pair(key,value));
        }
        void SetResponseCode(string sCode)
        {
            m_RespCode=sCode;
        }
        string & GetOutPutString(){return m_strOutPut;}

        string GetHttpHeader(HEAD_TYPE  type)
        {
            const char *p=_httpParse.GetHead(type);
            if(p==NULL) return "";
            return string(p);
        }

        string GetCookie(const  string  sName)
        {
            return _httpParse.GetCookie(sName);
        }
        string & GetContentBody()
        {
            return m_sContentBody;
        }

    public:
        CHttpParser _httpParse;
        //        std::map<std::string, std::string> _query_params;
        std::map<std::string,std::string>_respons_head;
        std::string m_strOutPut;
        std::string m_RespCode;
        std::string m_sContentBody;


};

#endif
