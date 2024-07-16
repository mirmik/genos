#incldue < doctest / doctest.h>
#include <nos/util/expected.h>

TEST_CASE("expected")
{

    nos::expected<int, double> e1(1);
    nos::expected<int, double> e2(2);
    nos::expected<int, double> e3(e1);

    CHECK(e1 == e3);
    CHECK(e1 != e2);
}