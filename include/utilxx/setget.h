#ifndef GENOS_SETTER_GETTER_H
#define GENOS_SETTER_GETTER_H

//Примитивы для написания интерфейса доступа к полям класса в setter/getter стиле.

#define SETTER(name,param) 					\
void name(decltype(param) value) {			\
	param = value;							\
} 							

#define VALUE_GETTER(name,param) 			\
auto name() const {							\
	return param;							\
} 

#define REFERENCE_GETTER(name,param)		\
auto & name() const {						\
	return param;							\
} 

#define CONSTREF_GETTER(name,param) 		\
auto const& name() const {					\
	return param;							\
} 

#define ACCESSOR(name,param)				\
	SETTER(name,param)						\
	CONSTREF_GETTER(name,param)					

#endif