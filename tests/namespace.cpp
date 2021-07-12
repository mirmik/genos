#include <doctest/doctest.h>
#include <genos/resource/namespace.h>
#include <genos/drivers/devns.h>
#include <genos/drivers/virtual/devnull.h>

#include <genos/mvfs.h>

struct namespace_head ns0, ns1;
struct devices_namespace  devns;
struct devnull devnull;

struct namespace_operations ns_ops = {};


TEST_CASE("namespace0") 
{
	namespace_init(&ns0, "/dev", &ns_ops);
	namespace_init(&ns1, "/proc", &ns_ops);

	struct namespace_head * ns = namespace_lookup("/proc/jaja-binks", NULL);	

	CHECK_EQ(ns, &ns1);

	namespace_deinit(&ns0);
	namespace_deinit(&ns1);
}

TEST_CASE("namespace1") 
{
	namespace_init(&ns0, "/dev", &ns_ops);
	namespace_init(&ns1, "/proc", &ns_ops);

	struct namespace_head * ns = namespace_lookup("/pr", NULL);	

	CHECK_EQ(ns, (struct namespace_head *)NULL);

	namespace_deinit(&ns0);
	namespace_deinit(&ns1);
}

TEST_CASE("devnull") 
{
	devices_namespace_init(&devns, "/dev");
	devnull_init(&devnull);
	
	file_head * filp;
	mvfs_lookup(&filp, "/dev/null");

	CHECK_EQ(filp, &devnull.dev.fil);

	namespace_deinit(&devns.ns);
}
