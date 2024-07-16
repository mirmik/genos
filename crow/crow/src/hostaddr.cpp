#include <crow/address.h>
#include <crow/hostaddr.h>
#include <crow/hostaddr_view.h>

crow::hostaddr_view crow::hostaddr::view()
{
    return hostaddr_view(_addr.data(), _addr.size());
}

crow::hostaddr::operator crow::hostaddr_view()
{
    return view();
}

crow::hostaddr::hostaddr(const crow::hostaddr_view &h)
{
    _addr = std::vector<uint8_t>((uint8_t *)h.data(),
                                 (uint8_t *)h.data() + h.size());
}

crow::hostaddr::hostaddr(const std::string &str) : hostaddr(crow::address(str))
{
}

std::string crow::hostaddr::to_string() const
{
    return view().to_string();
}

const crow::hostaddr_view crow::hostaddr::view() const
{
    return hostaddr_view(_addr.data(), _addr.size());
}