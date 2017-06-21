#ifndef GENOS_DELEGATE_SCHEDEE_H
#define GENOS_DELEGATE_SCHEDEE_H

#include <kernel/sched/automschedee.h>

class delegate_schedee : public autom_schedee {
public:
	delegate_schedee(delegate<void> dlg) : m_dlg(dlg) {  }
	~delegate_schedee() { }

private:
	virtual void execute() override {
		m_dlg();
	}

	delegate<void> m_dlg;

protected:
	//virtual void invalidate() override {}
};