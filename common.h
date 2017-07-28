// Author:  (norvallu)

#ifndef _TENCENT_PBRPC_COMMON_H_
#define _TENCENT_PBRPC_COMMON_H_

#include <google/protobuf/stubs/common.h>
#include <arpa/inet.h>

namespace std {}

namespace tencent {
namespace pbrpc {

#define TENCENT_PBRPC_VERSION "1.0.1"

/////////////// types /////////////
typedef ::google::protobuf::uint uint;

typedef ::google::protobuf::int8  int8;
typedef ::google::protobuf::int16 int16;
typedef ::google::protobuf::int32 int32;
typedef ::google::protobuf::int64 int64;

typedef ::google::protobuf::uint8  uint8;
typedef ::google::protobuf::uint16 uint16;
typedef ::google::protobuf::uint32 uint32;
typedef ::google::protobuf::uint64 uint64;

static const int32 kint32max = ::google::protobuf::kint32max;
static const int32 kint32min = ::google::protobuf::kint32min;
static const int64 kint64max = ::google::protobuf::kint64max;
static const int64 kint64min = ::google::protobuf::kint64min;
static const uint32 kuint32max = ::google::protobuf::kuint32max;
static const uint64 kuint64max = ::google::protobuf::kuint64max;

/////////////// util macros /////////////
#define TENCENT_PBRPC_PP_CAT(a, b) TENCENT_PBRPC_PP_CAT_I(a, b)
#define TENCENT_PBRPC_PP_CAT_I(a, b) a ## b

#define TENCENT_PBRPC_DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
    TypeName(const TypeName&);                       \
    void operator=(const TypeName&)

/////////////// logging and check /////////////
// default log level: ERROR
enum LogLevel {
    LOG_LEVEL_FATAL   = 0,
    LOG_LEVEL_ERROR   = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_NOTICE  = 3,
    LOG_LEVEL_INFO    = 3,
    LOG_LEVEL_TRACE   = 4,
    LOG_LEVEL_DEBUG   = 5,
};

namespace internal {
LogLevel get_log_level();
void set_log_level(LogLevel level);
void log_handler(LogLevel level, const char* filename, int line, const char *fmt, ...);
} // namespace internal

#define TENCENT_PBRPC_SET_LOG_LEVEL(level) \
    ::tencent::pbrpc::internal::set_log_level(::tencent::pbrpc::LOG_LEVEL_##level)

#define SLOG(level, fmt, arg...) \
    (::tencent::pbrpc::LOG_LEVEL_##level > ::tencent::pbrpc::internal::get_log_level()) ? \
            (void)0 : ::tencent::pbrpc::internal::log_handler( \
                    ::tencent::pbrpc::LOG_LEVEL_##level, __FILE__, __LINE__, fmt, ##arg) \

#define SLOG_IF(condition, level, fmt, arg...) \
    !(condition) ? (void)0 : ::tencent::pbrpc::internal::log_handler( \
            ::tencent::pbrpc::LOG_LEVEL_##level, __FILE__, __LINE__, fmt, ##arg)

#if defined( LOG )
#define SCHECK(expression) CHECK(expression)
#define SCHECK_EQ(a, b) CHECK_EQ(a, b)
#define SCHECK_NE(a, b) CHECK_NE(a, b)
#define SCHECK_LT(a, b) CHECK_LT(a, b)
#define SCHECK_LE(a, b) CHECK_LE(a, b)
#define SCHECK_GT(a, b) CHECK_GT(a, b)
#define SCHECK_GE(a, b) CHECK_GE(a, b)
#else
#define SCHECK(expression) \
    SLOG_IF(!(expression), FATAL, "CHECK failed: " #expression)
#define SCHECK_EQ(a, b) SCHECK((a) == (b))
#define SCHECK_NE(a, b) SCHECK((a) != (b))
#define SCHECK_LT(a, b) SCHECK((a) <  (b))
#define SCHECK_LE(a, b) SCHECK((a) <= (b))
#define SCHECK_GT(a, b) SCHECK((a) >  (b))
#define SCHECK_GE(a, b) SCHECK((a) >= (b))
#endif

#ifdef WIN32
        #define ntohll(x)     _byteswap_uint64 (x)
#define htonll(x)     _byteswap_uint64 (x)
#else
    #if __BYTE_ORDER == __BIG_ENDIAN
        #define ntohll(x)       (x)
        #define htonll(x)       (x)
    #else 
        #if __BYTE_ORDER == __LITTLE_ENDIAN
            #define ntohll(x)     __bswap_64 (x)
            #define htonll(x)     __bswap_64 (x)
        #endif 
    #endif  
#endif


} // namespace pbrpc
} // namespace tencent

#endif // _TENCENT_PBRPC_COMMON_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
