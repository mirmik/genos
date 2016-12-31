//Макрос decltypeof имитирует поведение typeof в С/С++ коде.
//Он используется в заголовочных файлах, которые одновременно могут 
//подключаться  как из С кода, так и из С++ кода.
//Макрос должен максимально повторять поведение typeof в С++ варианте.

//#include <sys/cdefs.h>


#if __cplusplus
#include <gxx/utility.h>
#define decltypeof(a) typename gxx::remove_reference<decltype(a)>::type
#else
#define decltypeof(a) typeof(a)
#endif