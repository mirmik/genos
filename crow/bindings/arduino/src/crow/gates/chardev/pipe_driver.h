#ifndef CROW_GATES_chardev_PIPE_H
#define CROW_GATES_chardev_PIPE_H

#include <crow/gates/chardevchannel.h>

namespace crow
{
    class chardev_pipe_driver : public crow::chardev_driver
    {
        int fd;

      public:
        chardev_pipe_driver() {}
        chardev_pipe_driver(int fd) { bind_fd(fd); }

        void bind_fd(int fd) {}

        void send(char *data, int size) override { write(fd, data, size); }

        void serve() override {}
    };
}

#endif