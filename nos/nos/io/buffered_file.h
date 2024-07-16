#ifndef NOS_IO_BUFFERED_FILE_H
#define NOS_IO_BUFFERED_FILE_H

#include <nos/io/iostream.h>
#include <nos/util/osutil.h>
#include <string>

namespace nos
{

    class buffered_file : public nos::iostream
    {
    protected:
        FILE *filp = nullptr;
        bool _auto_flush = false;

    public:
        buffered_file(FILE *f) : filp(f) {}
        buffered_file() : filp(nullptr) {}

        buffered_file(const char *path, const char *mode)
        {
            open(path, mode);
        }

        buffered_file(const std::string &path, const std::string &mode)
        {
            open(path.c_str(), mode.c_str());
        }

        buffered_file(const buffered_file &) = default;
        buffered_file &operator=(const buffered_file &) = default;

        void flush_on_write(bool b)
        {
            _auto_flush = b;
        }

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            auto ret = fwrite(ptr, sizeof(char), sz, filp);
            if (_auto_flush)
                fflush(filp);
            return ret;
        }

        nos::expected<size_t, nos::input_error> read(void *ptr,
                                                     size_t sz) override
        {
            return fread(ptr, sizeof(char), sz, filp);
        }

        bool good()
        {
            return filp != nullptr;
        }

        int open(const char *path, const char *mode)
        {
            filp = fopen(path, mode);
            return filp == NULL ? -1 : 0;
        }

        int fdopen(int fd, const char *mode = "rw")
        {
            filp = nos::osutil::fdopen(fd, mode);
            return filp == NULL ? -1 : 0;
        }

        int flush() override
        {
            return fflush(filp);
        }

        int close()
        {
            return fclose(filp);
        }

        int64_t fd() const
        {
            return nos::osutil::fileno(filp);
        }

        void seekg(size_t pos)
        {
            fseek(filp, pos, SEEK_SET);
        }

        int nonblock(bool en)
        {
            return nos::osutil::nonblock(fd(), en);
        }
    };
}

#endif
