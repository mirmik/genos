#ifndef GENOS_ATTACHED_SERVICE_H
#define GENOS_ATTACHED_SERVICE_H

#include <kernel/sched/SchedeeResource.h>
#include <kernel/service/Service.h>
#include <kernel/sched/Schedee.h>

namespace Genos {
	class AttachedService;

	//namespace Glue {
	//	AttachedService* constructAttachedService();
	//	void destroyAttachedService(AttachedService*);
	//}

	class AttachedService : public Service, public SchedeeResource {
	private:
		MessageList queries;
		MessageHeader* current;

	public:
		void receive() {
			if (current) current->release();

			if (!queries.empty()) {
				current = &*queries.begin();
			}
			
			else {
				parent->wait(SCHEDEE_BLOCKED_RECEIVE);
			}
		}

		int8_t receiveQuery(MessageHeader* message) override {
			dprln("AttachedService::receiveQuery");
		}

		//DEBUG
		AttachedService() : SchedeeResource(nullptr) {}

	protected:
		void destroy() override;

	public: 
		static AttachedService* allocate();
	};
}

#endif