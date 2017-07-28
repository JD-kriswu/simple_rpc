// Author:  (norvallu)

#ifndef _TENCENT_PBRPC_RPC_ERROR_CODE_H_
#define _TENCENT_PBRPC_RPC_ERROR_CODE_H_

namespace tencent {
namespace pbrpc {

enum RpcErrorCode {
    RPC_SUCCESS = 0,
    RPC_ERROR_PARSE_REQUEST_MESSAGE = 1,
    RPC_ERROR_PARSE_RESPONSE_MESSAGE = 2,
    RPC_ERROR_UNCOMPRESS_MESSAGE = 3,
    RPC_ERROR_COMPRESS_TYPE = 4,
    RPC_ERROR_NOT_SPECIFY_METHOD_NAME = 5,
    RPC_ERROR_PARSE_METHOD_NAME = 6,
    RPC_ERROR_FOUND_SERVICE = 7,
    RPC_ERROR_FOUND_METHOD = 8,
    RPC_ERROR_CHANNEL_BROKEN = 9,
    RPC_ERROR_CONNECTION_CLOSED = 10,  //���Ӵ���
    RPC_ERROR_REQUEST_TIMEOUT = 11, // request timeout
    RPC_ERROR_REQUEST_CANCELED = 12, // request canceled
    RPC_ERROR_SERVER_UNAVAILABLE = 13, // server un-healthy
    RPC_ERROR_SERVER_UNREACHABLE = 14, // server un-reachable
    RPC_ERROR_SERVER_SHUTDOWN = 15,
    RPC_ERROR_SEND_BUFFER_FULL = 16,
    RPC_ERROR_SERIALIZE_REQUEST = 17,
    RPC_ERROR_SERIALIZE_RESPONSE = 18,
    RPC_ERROR_RESOLVE_ADDRESS = 19,
    RPC_ERROR_CREATE_STREAM = 20,
    RPC_ERROR_NOT_IN_RUNNING = 21,
    RPC_ERROR_SERVER_BUSY = 22,
     RPC_ERROR_RECV_BUFFER_OVERFLOW= 23,  //���ջ��������
     RPC_ERROR_RECV_CHECK_COMPLETE= 24,  //����ʱ�����Լ�����
     RPC_ERROR_PEER_CLOSED= 25,  //����ʱ�Է��ر�����
     RPC_ERROR_SEND= 26,  //���ʹ���
     RPC_ERROR_SYS= 27,  //ϵͳ����
    // error code for listener
    RPC_ERROR_TOO_MANY_OPEN_FILES = 101,

    RPC_ERROR_UNKNOWN = 999,
    RPC_ERROR_FROM_USER = 1000,
}; // enum RpcErrorCode

// Convert rpc error code to human readable string.
const char* RpcErrorCodeToString(int error_code);

} // namespace pbrpc
} // namespace tencent

#endif // _TENCENT_PBRPC_RPC_ERROR_CODE_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
