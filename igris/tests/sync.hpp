#include <igris/sync/syslock.h>

LT_BEGIN_TEST(igris_test_suite, sync_test)
{
	system_lock();
	system_unlock();
}
LT_END_TEST(sync_test)
