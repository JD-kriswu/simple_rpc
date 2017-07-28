// Author:  (norvallu)

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include <common.h>

namespace tencent {
namespace pbrpc {
namespace internal {

static LogLevel s_log_level = ::tencent::pbrpc::LOG_LEVEL_ERROR;

LogLevel get_log_level()
{
    return s_log_level;
}

void set_log_level(LogLevel level)
{
    s_log_level = level;
}

void log_handler(LogLevel level, const char* filename, int line, const char *fmt, ...)
{
    static const char* level_names[] = { "FATAL", "ERROR", "WARNNING",
                                         "INFO", "TRACE", "DEBUG" };
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, 4096, fmt, ap);
    va_end(ap);
#if 0
    fprintf(stderr, "libpbrpc %s %s %s:%d] %s\n",
            level_names[level],
            boost::posix_time::to_simple_string(
                boost::posix_time::microsec_clock::local_time()).c_str(),
            filename, line, buf);
    fprintf(stderr, "libpbrpc %s %s:%d] %s\n",
            level_names[level],
            filename, line, buf);
    fflush(stderr);
    if (level == ::tencent::pbrpc::LOG_LEVEL_FATAL)
    {
//        abort();
    }

#endif

     //log_write(level, filename,line, "%s",buf);

}

} // namespace internal
} // namespace pbrpc
} // namespace tencent

/* vim: set ts=4 sw=4 sts=4 tw=100 */
