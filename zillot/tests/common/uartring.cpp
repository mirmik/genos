#include <doctest/doctest.h>
#include <zillot/mock/uart.h>
#include <zillot/common/uartring.h>

TEST_CASE("uartring") 
{
	char rxbuf[48];
	char txbuf[48];
	zillot::mock::uart u;
	zillot::uartring uring(&u, txbuf, rxbuf);
}