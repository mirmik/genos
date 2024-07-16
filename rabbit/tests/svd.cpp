#include <doctest/doctest.h>

#include <nos/print.h>
#include <ralgo/linalg/svd.h>

TEST_CASE("svd")
{
	float angle = M_PI/4;
	float arr[] = { 
		cosf(angle)/4,sinf(angle)/2,
		sinf(angle)/4,-cosf(angle)/2 };
	auto svdmat = ralgo::matrix_view_ro<float>(arr, 2, 2);
	auto svd = ralgo::svd(svdmat);
}