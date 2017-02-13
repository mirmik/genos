#ifndef GENOS_SETTER_GETTER_H
#define GENOS_SETTER_GETTER_H

#define SETTER(name,param) 					\
void name(decltype(param) value) {			\
	param = value;							\
} 							

#define GETTER(name,param) 					\
auto name() {								\
	return param;							\
} 

#define CONST_GETTER(name,param) 			\
const auto name() const {					\
	return param;							\
} 

#define SETGET(name,param)					\
	SETTER(name,param)						\
	GETTER(name,param)							

#endif