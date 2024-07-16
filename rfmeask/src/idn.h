#ifndef IDN_H
#define IDN_H

#include <array>
#include <string>

std::string system_idn();
void set_idn_from_string(const std::string &str);
void set_idn_from_array(const std::array<std::string, 4> idn);

#endif