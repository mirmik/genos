#include <igris/util/graycode.h>
#include <igris/util/size.h>

LT_BEGIN_TEST(igris_test_suite, util_graycode)
{
	CHECK_EQ(igris::graycode(0), 0b000);	
	CHECK_EQ(igris::graycode(1), 0b001);
	CHECK_EQ(igris::graycode(2), 0b011);
	CHECK_EQ(igris::graycode(3), 0b010);
	CHECK_EQ(igris::graycode(4), 0b110);	
	CHECK_EQ(igris::graycode(5), 0b111);
	CHECK_EQ(igris::graycode(6), 0b101);
	CHECK_EQ(igris::graycode(7), 0b100);

	CHECK_EQ(igris::graycode(8),  0b1100);	
	CHECK_EQ(igris::graycode(9),  0b1101);
	CHECK_EQ(igris::graycode(10), 0b1111);
	CHECK_EQ(igris::graycode(11), 0b1110);
	CHECK_EQ(igris::graycode(12), 0b1010);	
	CHECK_EQ(igris::graycode(13), 0b1011);
	CHECK_EQ(igris::graycode(14), 0b1001);
	CHECK_EQ(igris::graycode(15), 0b1000);

	CHECK_EQ(igris::graycode(16), 0b11000);	
	CHECK_EQ(igris::graycode(17), 0b11001);
	CHECK_EQ(igris::graycode(18), 0b11011);
	CHECK_EQ(igris::graycode(19), 0b11010);
	CHECK_EQ(igris::graycode(20), 0b11110);	
	CHECK_EQ(igris::graycode(21), 0b11111);
	CHECK_EQ(igris::graycode(22), 0b11101);
	CHECK_EQ(igris::graycode(23), 0b11100);

	CHECK_EQ(igris::graycode(24), 0b10100);	
	CHECK_EQ(igris::graycode(25), 0b10101);
	CHECK_EQ(igris::graycode(26), 0b10111);
	CHECK_EQ(igris::graycode(27), 0b10110);
	CHECK_EQ(igris::graycode(28), 0b10010);	
	CHECK_EQ(igris::graycode(29), 0b10011);
	CHECK_EQ(igris::graycode(30), 0b10001);
	CHECK_EQ(igris::graycode(31), 0b10000);
}
LT_END_TEST(util_graycode)


LT_BEGIN_TEST(igris_test_suite, util_size)
{
	char c[3];
	CHECK_EQ(igris::size(c), 3);
}
LT_END_TEST(util_size)
