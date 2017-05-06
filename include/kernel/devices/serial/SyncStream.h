#ifndef GENOS_SYNCSTREAM_H
#define GENOS_SYNCSTREAM_H

#include <kernel/event/WakeUpWaiter.h>

#include <kernel/devices/serial/FlagedStream.h>
#include <kernel/sched/Schedee.h>
#include <kernel/sched/Scheduler.h>

namespace Genos {
	class SyncStream : public FlagedStream {
	public:
		StateFlag txEmptyBuffer;
	
		virtual int io_write(const char* data, size_t size) = 0;
	
		int write(const char* data, size_t size) {
			__label__ resend;
			int sended = 0;
			
			resend:
			sended += io_write(data + sended, size - sended);
			if (sended < size) {
				auto sch = Genos::currentSchedee();
				if (sch && sch->is_subst()) {
					Genos::WakeUpWaiter wakeup(sch);
					atomic_section_enter();
					sch->wait();
					txEmptyBuffer.wait(wakeup);
					atomic_section_leave();
					Genos::Glue::displace(Genos::Glue::SubstitutionSchedeeType);
					goto resend;
				}
				else {
					while(!txEmptyBuffer.is_set());
					goto resend;
				}
			}
		}
	};
}

#endif 