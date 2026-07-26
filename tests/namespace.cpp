#include <doctest/doctest.h>
#include <genos/resource/lookup.h>
#include <genos/resource/namespace.h>
#include <string>

class test_namespace : public genos::namespace_manager
{
    int result;

public:
    int calls = 0;
    std::string internal_path;

    test_namespace(const char *path, int result)
        : namespace_manager(path), result(result)
    {
    }

    int lookup(genos::file_head **filp, const char *path) override
    {
        (void)filp;
        calls++;
        internal_path = path;
        return result;
    }
};

TEST_CASE("namespace requires a complete path component match")
{
    test_namespace proc("/proc", 41);
    test_namespace dev("/namespace-regression-dev", 42);
    genos::file_head *file = nullptr;

    CHECK_EQ(genos::mvfs_lookup(&file, "/pr"), -1);
    CHECK_EQ(genos::mvfs_lookup(&file, "/namespace-regression-data"), -1);
    CHECK_EQ(genos::mvfs_lookup(&file, "/namespace-regression-device"), -1);
    CHECK_EQ(proc.calls, 0);
    CHECK_EQ(dev.calls, 0);
}

TEST_CASE("namespace accepts exact and child paths")
{
    test_namespace ns("/namespace-test", 42);
    genos::file_head *file = nullptr;

    CHECK_EQ(genos::mvfs_lookup(&file, "/namespace-test"), 42);
    CHECK_EQ(ns.internal_path, "");

    CHECK_EQ(genos::mvfs_lookup(&file, "/namespace-test/device"), 42);
    CHECK_EQ(ns.internal_path, "device");
}

TEST_CASE("namespace selects the longest complete prefix")
{
    test_namespace parent("/namespace-tree", 1);
    test_namespace child("/namespace-tree/deep", 2);
    genos::file_head *file = nullptr;

    CHECK_EQ(genos::mvfs_lookup(&file, "/namespace-tree/deep/device"), 2);
    CHECK_EQ(parent.calls, 0);
    CHECK_EQ(child.calls, 1);
    CHECK_EQ(child.internal_path, "device");
}
