#ifndef GENOS_MESSAGE_MANAGER_H
#define GENOS_MESSAGE_MANAGER_H

#include <debug/dprint.h>

#include <kernel/csection.h>
#include <kernel/service/MsgTag.h>
#include <kernel/service/ServiceTable.h>
#include <gxx/pool.h>
#include <gxx/dlist.h>
#include <gxx/array.h>

#include <kernel/sched/AutomSchedee.h>

namespace Genos {
	int8_t transportQuery(MessageHeader* header);
	void transportAnswer(MessageHeader* header);
}

#endif