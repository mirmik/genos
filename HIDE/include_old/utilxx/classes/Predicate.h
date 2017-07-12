#ifndef GENOS_PREDICATE_H
#define GENOS_PREDICATE_H

template <typename ... Args>
class Predicate {
public:
	virtual bool operator()(Args ... args) = 0;
};

template <typename ... Args>
class FalsePredicate : public Predicate<Args ...> {
public:
	bool operator()(Args ... args) {
		return false;
	}	
};

#endif