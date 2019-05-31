#ifndef GENOS_SCHEDEE_API_H
#define GENOS_SCHEDEE_API_H

#include <genos/sched.h>

__BEGIN_DECLS

int msleep(unsigned int milliseconds);

int readdir(int fd, char* buf, size_t maxsz);

__END_DECLS

#endif