#ifndef NOS_IO__FDFILE_H
#define NOS_IO__FDFILE_H

#include <nos/io/iostream.h>
#include <nos/util/osutil.h>

namespace nos
{
    class file : public nos::iostream
    {
    private:
        int64_t m_fd = -1;
        std::chrono::nanoseconds _input_timeout = 0ns;

    public:
        file() = default;
        file(int64_t fd) : m_fd(fd) {}
        file(const char *path, int flags)
        {
            open(path, flags);
        }

        void set_fd(int64_t fd)
        {
            m_fd = fd;
        }

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            return nos::osutil::write(m_fd, ptr, sz);
        }

        nos::expected<size_t, nos::input_error> read(void *ptr,
                                                     size_t sz) override;

        bool good()
        {
            return m_fd >= 0;
        }

        void set_input_timeout(std::chrono::nanoseconds timeout)
        {
            this->_input_timeout = timeout;
        }

        std::chrono::nanoseconds input_timeout() const
        {
            return this->_input_timeout;
        }

        int64_t open(const char *path, int mode)
        {
            m_fd = nos::osutil::open(path, mode);
            return m_fd;
        }

        int flush() override
        {
            return 0;
        }

        virtual int close()
        {
            return nos::osutil::close(m_fd);
        }

        int64_t fd() const
        {
            return m_fd;
        }

        int nonblock(bool en)
        {
            return nos::osutil::nonblock(fd(), en);
        }
    };
}

#endif
