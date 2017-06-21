#ifndef GENOS_ABSTRACT_SCHEDEE_MANAGER_H
#define GENOS_ABSTRACT_SCHEDEE_MANAGER_H

#include <gxx/dlist.h>

namespace Genos {
	class Schedee;

	class AbstractSchedeeManager {
	public:
		virtual void runSchedee(Schedee& sch) = 0;
		virtual void stopSchedee(Schedee& sch) = 0;	
		virtual void finalSchedee(Schedee& sch) = 0;		
		virtual void zombieSchedee(Schedee& sch) = 0;		
		virtual void waitSchedee(Schedee& sch, uint8_t type) = 0;
		virtual void unwaitSchedee(Schedee& sch, uint8_t type) = 0;
		virtual void execute() = 0;
	};

	namespace Glue {
		AbstractSchedeeManager& systemSchedeeManager();
	}

}

#endif