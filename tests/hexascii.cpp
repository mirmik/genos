#include <doctest/doctest.h>
#include <igris/util/hexascii.h>

TEST_CASE("hexascii")
{
    const uint8_t arr[4] = {0xAB, 0xCD, 0xEF, 0x23};
    char res[8];
    uint8_t res2[4];

    hexascii_encode(arr, 4, res);

    CHECK_EQ(res[0], 'A');
    CHECK_EQ(res[1], 'B');
    CHECK_EQ(res[2], 'C');
    CHECK_EQ(res[3], 'D');
    CHECK_EQ(res[4], 'E');
    CHECK_EQ(res[5], 'F');
    CHECK_EQ(res[6], '2');
    CHECK_EQ(res[7], '3');

    hexascii_decode(res, 8, res2);

    CHECK_EQ(res2[0], 0xAB);
    CHECK_EQ(res2[1], 0xCD);
    CHECK_EQ(res2[2], 0xEF);
    CHECK_EQ(res2[3], 0x23);

    CHECK_EQ(hex2half('0'), 0);
    CHECK_EQ(hex2half('1'), 1);
    CHECK_EQ(hex2half('2'), 2);
    CHECK_EQ(hex2half('3'), 3);
    CHECK_EQ(hex2half('4'), 4);
    CHECK_EQ(hex2half('5'), 5);
    CHECK_EQ(hex2half('6'), 6);
    CHECK_EQ(hex2half('7'), 7);
    CHECK_EQ(hex2half('8'), 8);
    CHECK_EQ(hex2half('9'), 9);
    CHECK_EQ(hex2half('A'), 10);
    CHECK_EQ(hex2half('B'), 11);
    CHECK_EQ(hex2half('C'), 12);
    CHECK_EQ(hex2half('D'), 13);
    CHECK_EQ(hex2half('E'), 14);
    CHECK_EQ(hex2half('F'), 15);

    CHECK_EQ(half2hex(0), '0');
    CHECK_EQ(half2hex(1), '1');
    CHECK_EQ(half2hex(2), '2');
    CHECK_EQ(half2hex(3), '3');
    CHECK_EQ(half2hex(4), '4');
    CHECK_EQ(half2hex(5), '5');
    CHECK_EQ(half2hex(6), '6');
    CHECK_EQ(half2hex(7), '7');
    CHECK_EQ(half2hex(8), '8');
    CHECK_EQ(half2hex(9), '9');
    CHECK_EQ(half2hex(10), 'A');
    CHECK_EQ(half2hex(11), 'B');
    CHECK_EQ(half2hex(12), 'C');
    CHECK_EQ(half2hex(13), 'D');
    CHECK_EQ(half2hex(14), 'E');
    CHECK_EQ(half2hex(15), 'F');
}
