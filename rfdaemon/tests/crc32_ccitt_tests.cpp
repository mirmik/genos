#include "guard/guard.h"
#include "crc32_ccitt.h"

#include <string>
#include <cstdint>

TEST_CASE("crc32_ccitt: пустой буфер даёт 0")
{
    uint32_t crc = crc32_ccitt(nullptr, 0, 0);
    CHECK_EQ(crc, static_cast<uint32_t>(0));
}

TEST_CASE("crc32_ccitt: известное значение для строки 123456789")
{
    const std::string data = "123456789";
    uint32_t crc = crc32_ccitt(data.data(),
                               static_cast<uint32_t>(data.size()),
                               0);
    CHECK_EQ(crc, static_cast<uint32_t>(0xA66DC476));
}

TEST_CASE("crc32_ccitt: корректная обработка хвоста (длина не кратна 4)")
{
    const std::string data = "abcde"; // 5 байт: попадём и в основной цикл, и в хвост
    uint32_t crc = crc32_ccitt(data.data(),
                               static_cast<uint32_t>(data.size()),
                               0);
    CHECK_EQ(crc, static_cast<uint32_t>(0x7FD66312));
}
