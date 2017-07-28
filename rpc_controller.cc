// Author:  (norvallu)

#include <rpc_controller.h>
#include <google/protobuf/descriptor.h>
#include <rpc_error_code.h>
namespace tencent {
    namespace pbrpc {

        RpcController::RpcController(): _sequence_id(0)
                                        , _error_code(RPC_SUCCESS)
                                            , _is_done(NOT_DONE)
                                            , _is_request_sent(false)
                                            , _sent_bytes(0)
                                            , _is_start_cancel(false)
                                            , _is_sync(false)
                                            , _timeout_id(0)
                                            ,_servicetype("UDP")
                                            ,_port(0)
                                            ,_pRecvBuf(NULL)
                                            ,_iRecvBufSize(0)
        {

        }

        RpcController::~RpcController()
        {
        }


        void RpcController::SetRequestCompressType(CompressType compress_type)
        {
            _user_options.request_compress_type = compress_type;
        }

        CompressType RpcController::RequestCompressType()
        {
            return _user_options.request_compress_type != CompressTypeAuto ?
                _user_options.request_compress_type : _auto_options.request_compress_type;
        }

        void RpcController::SetResponseCompressType(CompressType compress_type)
        {
            _user_options.response_compress_type = compress_type;
        }

        CompressType RpcController::ResponseCompressType()
        {
            return _user_options.response_compress_type != CompressTypeAuto ?
                _user_options.response_compress_type : _auto_options.response_compress_type;
        }

        void RpcController::SetTimeout(int64 timeout)
        {
            _user_options.timeout = timeout;
        }

        int64 RpcController::Timeout() const
        {
            return _user_options.timeout > 0 ?
                _user_options.timeout : _auto_options.timeout;
        }

        bool RpcController::Failed() const
        {
            return _error_code != RPC_SUCCESS;
        }

        int RpcController::ErrorCode() const
        {
            return _error_code;
        }

        const std::string& RpcController::Reason() const
        {
            return _reason;
        }

        std::string RpcController::ErrorText() const
        {
            if (_reason.empty()) {
                return RpcErrorCodeToString(_error_code);
            } else {
                return RpcErrorCodeToString(_error_code)
                    + std::string(": ") + _reason;
            }
        }

        void RpcController::StartCancel()
        {
            // TODO to support
        }

        void RpcController::SetFailed(const std::string& reason)
        {
            SetFailed(RPC_ERROR_FROM_USER, reason);
        }

        bool RpcController::IsCanceled() const
        {
            // TODO to support
            return false;
        }

        void RpcController::NotifyOnCancel(google::protobuf::Closure* /* callback */)
        {
            // TODO to support
        }


        // -----------------------------------------------------------------
        // Used both in client and server.
        // -----------------------------------------------------------------
        void RpcController::SetSequenceId(uint64 sequence_id)
        {
            _sequence_id = sequence_id;
        }

        uint64 RpcController::SequenceId() const
        {
            return _sequence_id;
        }

        void RpcController::SetMethodId(const std::string& method_id)
        {
            _method_id = method_id;
        }

        const std::string& RpcController::MethodId() const
        {
            return _method_id;
        }


        void RpcController::SetFailed(int error_code, const std::string& reason)
        {
            _error_code = error_code;
            _reason = reason;
            if (_error_code == RPC_ERROR_REQUEST_TIMEOUT)
            {
                _reason += _is_request_sent ?
                    ": request already sent to remote" :
                    ": request not sent to remote";
            }
        }


        // -----------------------------------------------------------------
        // Used only in client.
        // -----------------------------------------------------------------
        bool RpcController::IsDone() const
        {
            return _is_done == DONE;
        }

        bool RpcController::IsRequestSent() const
        {
            return _is_request_sent;
        }

        int64 RpcController::SentBytes() const
        {
            return _sent_bytes;
        }

        bool RpcController::IsStartCancel() const
        {
            return _is_start_cancel;
        }

        void RpcController::PushDoneCallback(const InternalDoneCallback * callback)
        {
            SCHECK(callback);
            _done_callbacks.push_back(callback);
        }

        void RpcController::Done(int error_code, const std::string& reason)
        {
            // make sure that the callback is only called once.
            if (_is_done == NOT_DONE)
            {
                _is_done = DONE;
                SetFailed(error_code, reason);

                while (!_done_callbacks.empty())
                {
                    InternalDoneCallback *callback = _done_callbacks.back();
                    callback(this);
                    _done_callbacks.pop_back();
                }
            }
        }

        void RpcController::FillFromMethodDescriptor(const google::protobuf::MethodDescriptor* method)
        {
            _method_id = method->full_name();
            if (_user_options.timeout <= 0)
            {
                int64 timeout_in_ms = method->options().HasExtension(method_timeout) ?
                    method->options().GetExtension(method_timeout) :
                        method->service()->options().GetExtension(service_timeout);
                if (timeout_in_ms <= 0) {
                    // Just a protection, it shouldn't happen.
                    timeout_in_ms = 1;
                }
                _auto_options.timeout = timeout_in_ms;
            }
            if (_user_options.request_compress_type == CompressTypeAuto) {
                _auto_options.request_compress_type =
                    method->options().HasExtension(request_compress_type) ?
                    method->options().GetExtension(request_compress_type) :
                        CompressTypeNone;
            }
            if (_user_options.response_compress_type == CompressTypeAuto) {
                _auto_options.response_compress_type =
                    method->options().HasExtension(response_compress_type) ?
                    method->options().GetExtension(response_compress_type) :
                        CompressTypeNone;
            }
            _servicetype=method->service()->options().GetExtension(ServerType);
            _port=method->service()->options().GetExtension(port);
        }

        void RpcController::SetSync()
        {
            _is_sync = true;
        }

        bool RpcController::IsSync() const
        {
            return _is_sync;
        }

        std::string RpcController::GetServiceType()
        {
            return _servicetype;	
        }
        uint16_t RpcController::GetPort()
        {
            return _port;
        }

        void RpcController::SetRecvBuf(char *pBuf,int iSize)
        {
            _pRecvBuf=pBuf;
            _iRecvBufSize=iSize;
        }
        void RpcController::GetRecvBuf(char **pBuf,int *iSize)
        {
            *pBuf=_pRecvBuf;
            *iSize=_iRecvBufSize;
        }

        void RpcController::SetMsg(void * p){m_pMsg=p;}
        void * RpcController::GetMsg(){return m_pMsg;}

        void RpcController::SetErrMsg(  std::string &errMsg){ _errMsg=errMsg;}
        std::string & RpcController::GetErrMsg( ){ return _errMsg;}
    
    } // namespace pbrpc
} // namespace tencent

/* vim: set ts=4 sw=4 sts=4 tw=100 */
