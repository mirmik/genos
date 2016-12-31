#ifndef GENOS_STUB_H
#define GENOS_STUB_H

//#include "genos/debug/debug.h"

#ifndef __cplusplus

static void do_nothing() {  };

#else

template <typename R = void,typename ... V> 
static R do_nothing(V...) { return (R)0; };

struct ignore_t {
    template <class type>
    type& operator= (type& value) { return value; }
    template <class type>
    operator type() { return type(); }
};
static ignore_t ignore;

class DoNothing
{public:
	template <typename R = void,typename ... V> 
	R do_nothing(V...) { return (R)0; };
};

class Stub {
public:
	template<typename Archive>
	void save(Archive& ar) {};

	template<typename Archive>
	void load(Archive& ar) {};

};

#endif //__cplusplus	

#endif 