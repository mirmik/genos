#include <doctest/doctest.h>
#include <igris/util/bitscopy.h>
#include <igris/dprint.h>

TEST_CASE("bitscopy") 
{
	uint8_t abuf[8] = { 0, 0, 0, 0, 0, 0, 0 };	
	uint8_t bbuf[8] = { 0xAB, 0xAB, 0xAB, 0,0,0,0,0 };

	bitscopy(abuf, 1, bbuf, 0, 22);
}