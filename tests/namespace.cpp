#include <doctest/doctest.h>
#include <genos/resource/namespace.h>

struct namespace_head ns0, ns1;

struct namespace_operations ns_ops = {};

TEST_CASE("namespace") 
{
	namespace_init(&ns0, "/dev", &ns_ops);
	namespace_init(&ns1, "/proc", &ns_ops);	
}
