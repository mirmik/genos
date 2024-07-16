#include <igris/sync/syslock.h>

volatile unsigned char __system_lock_is_locked = 0;
