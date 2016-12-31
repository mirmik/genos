#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include "inttypes.h"
#include "gxx/DList.h"
#include "util/bits.h"

#include "kernel/signals.h"

#define SCHEDEE_READY 			0x0001
#define SCHEDEE_HELD			0x0002
#define SCHEDEE_WAIT 			0x0004
#define SCHEDEE_ZOMBIE			0x0020

#define SCHEDEE_STATE_MASK		0x0027
//#define SCHEDEE_STATE_WAIT_MASK	0x001E

//Флаг указывает, что передачу управления диспетчеру при
//вызове блокирующих операций процесс берёт на себя.
//Кроме того, он не может вытесняться.
#define SCHEDEE_NONPREEMPTIBLE	0x1000

namespace Kernel {	
	
	class schedee {
	public:
		dlist_head statelnk;
		
		sig_t signals;
		sig_t signals_mask;

		uint16_t flags;
		int8_t dyn_prio;
		int8_t sta_prio;

		inline bool is_ready() {
			return bits_mask(flags, SCHEDEE_READY);
		}

		void start();
		void stop();

		virtual void execute() = 0;
	};
}

#endif