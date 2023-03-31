#ifndef GENOS_RESOURCE_OPENRES_H
#define GENOS_RESOURCE_OPENRES_H

#include <genos/resource/file_head.h>
#include <genos/resource/lookup.h>
#include <igris/compiler.h>
#include <sys/types.h>

namespace genos
{
    class openres
    {
    public:
        genos::file_head *file = nullptr;
        int pos = 0;

    public:
        openres() = default;
        openres(const openres &) = delete;
        openres &operator=(const openres &) = delete;
        openres(openres &&) = default;
        openres &operator=(openres &&) = default;

        void init(genos::file_head *file)
        {
            this->file = file;
            pos = 0;
        }

        int open(const char *path, int flags)
        {
            if (file != nullptr)
                return -1;
            int ret = mvfs_lookup(&file, path);
            return ret;
        }

        void open_as(genos::openres &res)
        {
            file = res.file;
            file->on_open();
        }

        int release()
        {
            if (file == nullptr)
                return -1;
            file->on_release();
        }

        ssize_t write(const void *data, unsigned int size)
        {
            if (file == nullptr)
                return -1;
            return file->write(data, size);
        }

        ssize_t read(void *read, unsigned int size)
        {
            if (file == nullptr)
                return -1;
            return file->read(read, size);
        }

        int wait_for_avail()
        {
            if (file == nullptr)
                return -1;
            return file->wait_for_avail();
        }
    };
}

#endif