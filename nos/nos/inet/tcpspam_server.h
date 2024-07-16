#ifndef NOS_TCPSPAM_SERVER_H
#define NOS_TCPSPAM_SERVER_H

#include <algorithm>
#include <list>
#include <nos/inet/tcp_server.h>
#include <nos/io/ostream.h>
#include <string.h>

namespace nos
{
    namespace inet
    {
        class tcpspam_server : public inet::tcp_server
        {
            std::list<nos::inet::tcp_client> clients = {};

        private:
            int __send(const char *str)
            {
                return __send(str, strlen(str));
            };

            int __send(const char *str, size_t n)
            {
                while (true)
                {
                    nos::inet::tcp_client newsock = accept();
                    if (!newsock.good())
                        break;
                    clients.push_back(newsock);
                }

                int ret = 0;
                auto it = clients.begin();
                decltype(it) cit;
                auto eit = clients.end();
                while (it != eit)
                {
                    cit = it;
                    ++it;
                    ret = *cit->write(str, n);
                    if (ret < 0)
                    {
                        clients.erase(cit);
                    }
                }

                return ret;
            }

        public:
            tcpspam_server() = default;
            tcpspam_server(int port)
            {
                start(port);
            }

            int start(int port)
            {
                inet::tcp_server::init();
                inet::tcp_server::reusing(true);
                inet::tcp_server::bind("0.0.0.0", port);
                inet::tcp_server::listen(10);
                inet::tcp_server::nonblock(true);
                return 0;
            }

            nos::expected<size_t, nos::output_error> write(const void *str,
                                                           size_t sz) override
            {
                return __send((char *)str, sz);
            }

            void drop_all()
            {
                for (auto &c : clients)
                {
                    c.close();
                }
                clients.clear();
            }
        };
    }
}

#endif
