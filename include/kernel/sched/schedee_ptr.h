#ifndef KERNEL_SCHEDEE_H
#define KERNEL_SCHEDEE_H

class schedee;

class schedee_ptr {
	schedee* sch;
public:
	schedee_ptr(schedee* ptr);
	schedee_ptr(const schedee_ptr& rhs);
	~schedee_ptr();


	schedee* operator->();
	schedee& operator*();

	void addref();

private:
	//schedee* raw() { return sch; }
	//friend class schedee;
};

#endif