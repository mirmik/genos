#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <gxx/allocator.h>
#include <gxx/DList.h>
#include <genos/sigslot/delegate.h>
#include <util/stub.h>
#include <kernel/sched/schedee_ptr.h>

#include <kernel/service/service.h>

class schedee;
schedee* current_schedee();
void current_schedee(schedee* sch);

#define SCHEDULE_RESUME 1
#define SCHEDULE_REPEAT 0

static constexpr uint8_t SCHEDEE_STATE_INIT = 0x00;
static constexpr uint8_t SCHEDEE_STATE_RUN = 0x01;
static constexpr uint8_t SCHEDEE_STATE_WAIT = 0x02;
static constexpr uint8_t SCHEDEE_BLOCKED_SEND = 0x03;
static constexpr uint8_t SCHEDEE_BLOCKED_RECEIVE = 0x04;
static constexpr uint8_t SCHEDEE_STATE_FINAL = 0x05;
static constexpr uint8_t SCHEDEE_STATE_ZOMBIE = 0x06;
static constexpr uint8_t SCHEDEE_BLOCKED_STOP = 0x07;

static constexpr uint8_t SCHEDEE_STATE_MASK = 0x0F;


static constexpr uint8_t SCHEDEE_FINAL_DEALLOCATE = 0x80;
static constexpr uint8_t SCHEDEE_FINAL_RELEASE = 0x40;

class schedee : public service {
public:
	dlist_head lnk;
	uint8_t prio = 0;
	uint8_t flags = 0;

//	uint8_t ref = 1;

//	schedee_ptr parent;
	//void* argument;
	//void* result;

	void(*final_callback)(schedee* sch) = nullptr;

	//schedee_ptr инициализируется без инкремента счетчика ссылок,
	//т.к. инициализируется raw указателем.
	schedee();

 	virtual ~schedee() { }

	//schedee& Result(void* res);
	//void* Result();
	
	schedee& Run();
	schedee& Stop();
	void Exit();
	//void release();

	schedee& final_deallocate(bool en);
	schedee& final_release(bool en);
	
	schedee& final_handler(void(*)(schedee*));

	template<typename F> 
	schedee& final_handler(F f) {
		return final_handler((void(*)(schedee*)) f);
	}

	schedee& final_argument(void*);

	bool Completed();
	bool operator==(const schedee& other) { return this == &other; }

	virtual uint8_t execute() = 0;
	//virtual void invalidate() = 0;
	void final();
};

//static void resume_parent_schedee(schedee* sch) {
//	((schedee*)sch->argument) -> Run();
//}

//extern gxx::DList<schedee, &schedee::lnk> waitlist;
//static void resume_parent_schedee_safe(schedee* sch) {
//	if ( gxx::find(waitlist.begin(), waitlist.end(), *sch) != waitlist.end()) 
//		resume_parent_schedee(sch);
//}



#endif