#ifndef KERNEL_SIGNALS_H
#define KERNEL_SIGNALS_H

typedef uint16_t sig_t;
class schedee;

#define SIGTERM 0x01
#define SIGINT 0x02
#define SIGSTOP 0x04

namespace Kernel {
//	void send(schedee* sch, sig_t sig);
//	void send(id_t pid, sig_t sig);
};

#endif