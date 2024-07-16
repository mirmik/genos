#include <doctest/doctest.h>
#include <crow/tower.h>

TEST_CASE("start-stop") 
{
	crow::start_spin();
	crow::stop_spin(true);
} 