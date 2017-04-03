#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <stdint.h>
#include <datastruct/dlist_head.h>

#define SCHEDULE_RESUME 1
#define SCHEDULE_REPEAT 0

static const uint8_t SCHEDEE_STATE_INIT = 0x00;
static const uint8_t SCHEDEE_STATE_RUN = 0x01;

static const uint8_t SCHEDEE_STATE_WAIT = 0x02;
static const uint8_t SCHEDEE_BLOCKED_SEND = 0x03;
static const uint8_t SCHEDEE_BLOCKED_RECEIVE = 0x04;
static const uint8_t SCHEDEE_STATE_FINAL = 0x05;
static const uint8_t SCHEDEE_STATE_ZOMBIE = 0x06;
static const uint8_t SCHEDEE_BLOCKED_STOP = 0x07;

static const uint8_t SCHEDEE_STATE_MASK = 0x0F;

static const uint8_t SCHEDEE_FINAL_DEALLOCATE = 0x80;
static const uint8_t SCHEDEE_FINAL_RELEASE = 0x40;

namespace Genos {

	class Schedee {
	public:
		struct dlist_head schlnk;
		uint8_t prio;
		uint8_t flags;

	public:
		Schedee(uint8_t prio) : prio(prio), flags(SCHEDEE_STATE_INIT) {};

		void run();
		void stop();
		//void exit();

		void set_state_wait(uint8_t state);
		void set_state_run();
		void set_state_final();
		void set_state_zombie();
		
		bool state_is(uint8_t state);
		
		void set_final_deallocate(bool en);
		void set_final_release(bool en);
		
		bool is_running();
		bool is_waiting();
		bool is_zombie();
		bool is_final_deallocated();
		bool is_final_release();

		virtual uint8_t execute() {panic("not suported operation");};
		virtual uint8_t engage() {panic("not suported operation");};
		virtual uint8_t displace() {panic("not suported operation");};
		virtual uint8_t lastexit() {panic("not suported operation");};
		virtual uint8_t destructor() {panic("not suported operation");};

	};

	//void schedee_exit();

	Schedee* current_schedee();
	void set_current_schedee(Schedee* sch);

	void finalize_schedee(Schedee* sch);
}

#endif