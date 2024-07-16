#include <idn.h>
#include <nos/util/string.h>

std::string scpi_idn[4];

std::string system_idn()
{
    return scpi_idn[0] + "," + scpi_idn[1] + "," + scpi_idn[2] + "," +
           scpi_idn[3];
}

void set_idn_from_string(const std::string &str)
{
    auto lst = nos::split(str, ',');
    for (unsigned int i = 0; i < lst.size(); i++)
        scpi_idn[i] = lst[i];
}

void set_idn_from_array(const std::array<std::string, 4> idn)
{
    for (unsigned int i = 0; i < idn.size(); i++)
        scpi_idn[i] = idn[i];
}
