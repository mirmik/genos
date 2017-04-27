#ifndef GENOS_SCHEDEE_RESOURCE_H
#define GENOS_SCHEDEE_RESOURCE_H

#include <gxx/dlist.h>
//#include <kernel/resources/ManagedObject.h>

#include <gxx/vector.h>
#include <utilxx/setget.h>

namespace Genos {
	class Schedee;

	class SchedeeResource {
	public:
		Schedee* parent = nullptr;

	public:
		dlist_head reslnk;

	public:
		SchedeeResource() {
			dlist_init(&reslnk);
		}

		void setParent(Schedee* parent);
		void unbindFromParent();

		virtual void releaseResource() = 0;
	};

	using SchedeeResourceList 
		= gxx::dlist<SchedeeResource, &SchedeeResource::reslnk>;
}

#endif