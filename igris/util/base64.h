#ifndef IGRIS_UTIL_BASE64_H
#define IGRIS_UTIL_BASE64_H

#include <string>
#include <cstdint>

namespace igris
{
    // bool is_base64(unsigned char c);
    std::string base64_encode(const uint8_t *indata, size_t size);
    std::string base64_encode(std::string const &str);
    std::string base64_decode(std::string const &encoded_string);
    std::string base64url_encode(const uint8_t *indata, size_t size);
    std::string base64url_encode(std::string const &str);
    std::string base64url_decode(std::string const &encoded_string);
}

#endif
