#include <sched/Schedee.h>
#include <resource/descriptor.h>
#include <err/panic.h>

namespace genos {
	int push_descriptor(Genos::Schedee* sch) {
		gxx::vector<descriptor> descs;
		for (auto it = descs.begin(), end = descs.end(); it != end; it++) {
			if (it->type == genos::descriptor::DescType::Nil) return it - descs.begin();
		}
		descs.resize(descs.size() + 1);
		return descs.size() - 1;
	} 

	int open_test_resource(test_resource& res) {
		Genos::Schedee* sch = Genos::currentSchedee();
		assert(sch);
		int no = push_descriptor(sch);
		new (&sch->descriptors[no]) descriptor(descriptor::DescType::TestResource, 0, &res);
		res.open();
		return no;
	}
	
	void close_test_resource(descriptor& desc) {
		reinterpret_cast<test_resource*>(desc.body)->release();
	}

	void close_descriptor(descriptor& desc) {
		//dprln("CLOSE_DESCRIPTOR");
		switch (desc.type) {
			case descriptor::DescType::Nil: break;
			case descriptor::DescType::File: dprln("CLOSE_FILE"); break;
			case descriptor::DescType::Messenger: dprln("CLOSE_MESSENGER"); break;
			case descriptor::DescType::TestResource: close_test_resource(desc); break;
			case descriptor::DescType::UserResource: dprln("CLOSE_USER_RESOURCE"); break;
			default: panic("unregistred descriptor type");
		}
	}

	descriptor* get_descriptor(int no) {
		Genos::Schedee* sch = Genos::currentSchedee();
		if (!sch 
			|| no >= sch->descriptors.size() 
			|| sch->descriptors[no].type == descriptor::DescType::Nil
		) return nullptr;
		return &sch->descriptors[no];
	}
}