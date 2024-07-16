#ifndef NOS_IO_SOCKET2_H
#define NOS_IO_SOCKET2_H

#include <nos/inet/hostaddr.h>
#include <nos/io/file.h>
#include <string.h>

namespace nos
{
    namespace inet
    {
        class socket : public nos::file
        {
        public:
            socket() = default;
            socket(int fd) : file(fd) {}
            socket(const socket &oth) = default;
            socket(socket &&oth) = default;
            socket &operator=(const socket &oth) = default;
            socket &operator=(socket &&oth) = default;

            bool good()
            {
                return fd() >= 0;
            }

            nos::expected<size_t, nos::output_error>
            send(const void *data, size_t size, int flags);
            nos::expected<size_t, nos::input_error>
            recv(char *data, size_t size, int flags);

            int init(int domain, int type, int proto); // posix ::socket
            int bind(const hostaddr &haddr, uint16_t port, int family);
            int connect(const hostaddr &haddr, uint16_t port, int family);
            // int clean();
            int listen(int conn);

            int nodelay(bool en);
            int nonblock(bool en);
            int dontroute(bool en);
            int reusing(bool en);

            int close_socket();
            int close() override
            {
                return close_socket();
            }
            bool is_closed();

            bool operator==(const nos::inet::socket &oth) const
            {
                return fd() == oth.fd();
            }
            bool operator!=(const nos::inet::socket &oth) const
            {
                return fd() != oth.fd();
            }
            bool operator>(const nos::inet::socket &oth) const
            {
                return fd() > oth.fd();
            }
            bool operator<(const nos::inet::socket &oth) const
            {
                return fd() < oth.fd();
            }

            virtual ~socket() = default;
        };
    }
}

namespace std
{
    template <> struct hash<nos::inet::socket>
    {
        typedef nos::inet::socket argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const &s) const
        {
            return std::hash<int64_t>()(s.fd());
        }
    };
}

#endif
