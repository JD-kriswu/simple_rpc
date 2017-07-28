// Author:  (norvallu)

#ifndef _TENCENT_PBRPC_RPC_MESSAGE_HEADER_H_
#define _TENCENT_PBRPC_RPC_MESSAGE_HEADER_H_

#include "common.h" 
 #include <arpa/inet.h>
namespace tencent {
namespace pbrpc {

#define TENCENT_RPC_MAGIC 1095126867u

// total 16 bytes
struct RpcMessageHeader {
    union {
        char    magic_str[4];
        uint32  magic_str_value;
    };                    // 4 bytes
    int32   meta_size;    // 4 bytes
    int64   data_size;    // 8 bytes

    RpcMessageHeader()
        : magic_str_value(ntohl(TENCENT_RPC_MAGIC))
        , meta_size(0), data_size(0){}

    bool CheckMagicString() const
    {
        return magic_str_value == ntohl(TENCENT_RPC_MAGIC);
    }
};

} // namespace pbrpc
} // namespace tencent

#endif // _TENCENT_PBRPC_RPC_MESSAGE_HEADER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
