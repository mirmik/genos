#ifndef GENOS_SETTER_GETTER_H
#define GENOS_SETTER_GETTER_H

#define SETTER(name,param) 					\
void name(decltype(param) value) {			\
	param = value;							\
} 							

#define GETTER(name,param) 					\
decltype(param) name() {					\
	return param;							\
} 

#define SETGET(name,param)					\
	SETTER(name,param)						\
	GETTER(name,param)							

#endif