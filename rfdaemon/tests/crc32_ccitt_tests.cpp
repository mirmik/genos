#include "crc32_ccitt.h"
#include "guard/guard.h"

#include <cstdint>
#include <string>

TEST_CASE("crc32_ccitt: пустой буфер даёт начальное значение")
{
    uint32_t crc = crc32_ccitt(nullptr, 0, 0);
    CHECK_EQ(crc, static_cast<uint32_t>(0));
}

TEST_CASE("crc32_ccitt: известное значение для строки 123456789")
{
    // Стандартное значение CRC32 для "123456789" = 0xCBF43926
    const std::string data = "123456789";
    uint32_t crc =
        crc32_ccitt(data.data(), static_cast<uint32_t>(data.size()), 0);
    CHECK_EQ(crc, static_cast<uint32_t>(0xCBF43926));
}

TEST_CASE("crc32_ccitt: корректная обработка хвоста (длина не кратна 4)")
{
    // CRC32 для "abcde" = 0x8587D865
    const std::string data = "abcde";
    uint32_t crc =
        crc32_ccitt(data.data(), static_cast<uint32_t>(data.size()), 0);
    CHECK_EQ(crc, static_cast<uint32_t>(0x8587D865));
}
