#ifndef GXX_IOVEC_OPS_H
#define GXX_IOVEC_OPS_H

#include <gxx/math/util.h>
#include <sys/uio.h>

__BEGIN_DECLS

/*static inline int memcpy_toiovec(struct iovec *iov, unsigned char *kdata, int
len)
{
    while (len > 0) {
        if (iov->iov_len) {
            int copy = min_t(unsigned int, iov->iov_len, len);
            memcpy(iov->iov_base, kdata, copy);
            kdata += copy;
            len -= copy;
            iov->iov_len -= copy;
            iov->iov_base += copy;
        }
        iov++;
    }

    return 0;
}*/

/*static inline int memcpy_fromiovec(unsigned char *kdata, struct iovec *iov,
int len)
{
    while (len > 0) {
        if (iov->iov_len) {
            int copy = min_t(unsigned int, len, iov->iov_len);
            memcpy(kdata, iov->iov_base, copy);
            len -= copy;
            kdata += copy;
            iov->iov_base += copy;
            iov->iov_len -= copy;
        }
        iov++;
    }

    return 0;
}*/

__END_DECLS

#endif
