#include <cstring>
#include <functional>
#include <idn.h>
#include <ifaddrs.h>
#include <iomanip>
#include <net/if.h>
#include <netpacket/packet.h>
#include <nos/util/string.h>
#include <sstream>
#include <string>

std::string scpi_idn[4];

std::string get_first_mac_address()
{
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        return "";
    }

    std::string mac;
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_PACKET)
            continue;

        if (ifa->ifa_flags & IFF_LOOPBACK)
            continue;

        struct sockaddr_ll *s = (struct sockaddr_ll *)ifa->ifa_addr;
        if (s->sll_halen == 6)
        {
            char buf[18];
            snprintf(buf,
                     sizeof(buf),
                     "%02x:%02x:%02x:%02x:%02x:%02x",
                     s->sll_addr[0],
                     s->sll_addr[1],
                     s->sll_addr[2],
                     s->sll_addr[3],
                     s->sll_addr[4],
                     s->sll_addr[5]);
            mac = buf;
            break;
        }
    }

    freeifaddrs(ifaddr);
    return mac;
}

std::string unical_identifier()
{
    std::string mac = get_first_mac_address();
    if (mac.empty())
        return "00000000";

    // Удаляем двоеточия или дефисы, если они есть
    mac.erase(std::remove(mac.begin(), mac.end(), ':'), mac.end());
    mac.erase(std::remove(mac.begin(), mac.end(), '-'), mac.end());

    // Если длина меньше 8 символов, дополняем нулями слева
    while (mac.length() < 8)
        mac = "0" + mac;

    // Обрезаем до 8 символов
    return mac.substr(0, 8);
}

std::string system_idn()
{
    return scpi_idn[0] + "," + scpi_idn[1] + "," + scpi_idn[2] + "," +
           scpi_idn[3];
}

void set_idn_from_string(const std::string &str)
{
    const std::array<std::string, 4> idn;

    auto lst = nos::split(str, ',');
    for (unsigned int i = 0; i < lst.size(); i++)
    {
        if (i < idn.size())
            scpi_idn[i] = lst[i];
        else
            break;
    }

    set_idn_from_array(idn);
}

void set_idn_from_array(const std::array<std::string, 4> idn)
{
    for (unsigned int i = 0; i < idn.size(); i++)
        scpi_idn[i] = idn[i];

    scpi_idn[2] = unical_identifier().substr(0, 8);
}
