#include <igris/util/signature.h>

class Signature0
{
public:
	int test();
};

class Signature1
{
public:
};

IGRIS_SIGNATURE_ATTRIBUTE_CHECKER(has_test, test)

TEST_CASE("igris_test_suite")
{
	CHECK(has_test<Signature0>() == true);
	CHECK(has_test<Signature1>() == false);
}
