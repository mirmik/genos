/** @file */

#include "tcp_client.h"

std::map<nos::inet::netaddr, crowker_implementation::tcp_client>
    crowker_implementation::tcp_client::allsubs;
void crowker_implementation::tcp_client::publish(const std::string &theme,
                                                 const std::string &data,
                                                 options opts)
{
    (void)opts;
    std::string str = nos::format("p{:f02>}{}{:f06>}{}", theme.size(), theme,
                                  data.size(), data);
    sock->write(str.data(), str.size());
}