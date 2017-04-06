#ifndef GENOS_LINUX_ATOMIC
#define GENOS_LINUX_ATOMIC

#include <asm/atomic_lock.h>
#include <kernel/csection.h>

std::mutex lock_atomic::atomic_mutex;

#endif