#include <nos/io/file.h>
#ifdef __WIN32__
#include <winsock2.h>
#define pipe(ptr) _pipe(ptr, 1024, 0)
using suseconds_t = int32_t;
using socklen_t = int32_t;
#elif defined(_MSC_VER)
#include <winsock2.h>
#define pipe(ptr) _pipe(ptr, 1024, 0)
using suseconds_t = int32_t;
using socklen_t = int32_t;
#else
#include <sys/select.h>
#include <sys/time.h>
#endif

#include <nos/fprint.h>
#include <nos/util/osutil.h>

nos::expected<size_t, nos::input_error> nos::file::read(void *ptr, size_t sz)
{
    assert(m_fd >= 0);
    assert(fd() >= 0);

    if (input_timeout() == 0ns)
    {
        int sts = nos::osutil::read(fd(), ptr, sz);

        if (sts <= 0)
        {
            return nos::input_error::eof();
        }

        return sts;
    }

    auto ns = input_timeout().count();

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd(), &fds);

    struct timeval tv;
    tv.tv_sec = (long)(ns / 1000000000);
    tv.tv_usec = (ns % 1000000000) / 1000;

    // nonblock(true);
    int ret = select((int)(fd() + 1), &fds, nullptr, nullptr, &tv);

    if (ret < 0)
    {
        return nos::input_error::closed();
    }

    if (ret == 0)
    {
        return nos::input_error::timeout();
    }

    auto read_sts = nos::osutil::read(fd(), ptr, sz);

    if (read_sts < 0)
        return input_error::closed();

    return read_sts;
}