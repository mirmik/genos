#ifndef GENOS_EVENTED_H
#define GENOS_EVENTED_H

template <typename ... Args>
class evented {
	virtual void emit() = 0;
}

#endif