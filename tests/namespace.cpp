#include <doctest/doctest.h>
#include <genos/resource/namespace.h>

struct namespace_head ns0, ns1;

struct namespace_operations ns_ops = {};

TEST_CASE("namespace0") 
{
	namespace_init(&ns0, "/dev", &ns_ops);
	namespace_init(&ns1, "/proc", &ns_ops);

	struct namespace_head * ns = namespace_lookup("/proc/jaja-binks");	

	CHECK_EQ(ns, &ns1);

	namespace_deinit(&ns0);
	namespace_deinit(&ns1);
}

TEST_CASE("namespace1") 
{
	namespace_init(&ns0, "/dev", &ns_ops);
	namespace_init(&ns1, "/proc", &ns_ops);

	struct namespace_head * ns = namespace_lookup("/pr");	

	CHECK_EQ(ns, (struct namespace_head *)NULL);

	namespace_deinit(&ns0);
	namespace_deinit(&ns1);
}
