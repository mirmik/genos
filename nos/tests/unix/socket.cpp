#include <doctest/doctest.h>
#include <nos/inet/socket.h>
#include <nos/inet/tcp_socket.h>

#include <unordered_set>

TEST_CASE("socket") 
{
	nos::inet::socket a(22);
	nos::inet::socket b(22);
	std::unordered_set<nos::inet::socket> set;

	CHECK_EQ(a, b);
	CHECK_NE(std::hash<nos::inet::socket>()(a), 0);
}