#ifndef NOS_INIT_PRIORITY_H
#define NOS_INIT_PRIORITY_H

//Макросы для изменения порядка инициализации
//Врядли совместимо.

#ifndef _MSC_VER
#define NOS_PRIORITY_INITIALIZATION_SUPER 	__attribute__((init_priority(140)))
#define NOS_PRIORITY_INITIALIZATION 		__attribute__((init_priority(1000)))
#else
#define NOS_PRIORITY_INITIALIZATION_SUPER
#define NOS_PRIORITY_INITIALIZATION
#endif

#endif
