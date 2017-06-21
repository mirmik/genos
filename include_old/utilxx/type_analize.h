
//Анализ типа переменных и классов на основе переменной компилятора __PRETTY_FUNCTION__.

#ifndef TYPE_ANALIZE_H
#define TYPE_ANALIZE_H

template<typename ... T>
const char* pretty_function()
	{return __PRETTY_FUNCTION__;};
	
template<typename T>
const char* pretty_function(T obj)
	{return __PRETTY_FUNCTION__;};

#define pretty_that_function() debug_print(__PRETTY_FUNCTION__)

#endif