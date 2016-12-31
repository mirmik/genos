#ifndef STATIC_STACK_H
#define STATIC_STACK_H
	
#include "stdlib.h"
	
//Ни делает ничего, резервирует место.
template<size_t N>
class static_stack{
private:
	uint8_t size[N];

public:
	//Возврат указателя на последний байт стека (стек растет 
	//от старших байт к младшим).  
	uintptr_t top()
	{
		return (uintptr_t)(this + 1) - 1; 
	};		
};
	
#endif