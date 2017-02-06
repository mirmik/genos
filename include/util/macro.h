#ifndef UTIL_MACRO_H
#define UTIL_MACRO_H

//compat names
#define STRINGIFY(x) __MACRO_STRING(x)
#define CAT(x,y) __MACRO_CONCATE(x,y)

//Раскрытие
#define MACRO_EXPAND(m) m

//Косвенное превращение в строку
#define MACRO_STRING(...) __MACRO_STRING(__VA_ARGS__)
#define __MACRO_STRING(...) # __VA_ARGS__

//Косвенная конкатенация
#define MACRO_CONCAT(m1, m2) __MACRO_CONCATE(m1,m2)
#define __MACRO_CONCATE(m1, m2) m1##m2

//Вызов функции
#define MACRO_INVOKE(m, ...) m(__VA_ARGS__)

//Вызов функции-члена
#define MACRO_MEMBER_INVOKE(o, m, ...) o.m(__VA_ARGS__)

//Уникализация имени добавления "_at_line_[LINE]".
#define MACRO_GUARD(symbol) __MACRO_GUARD(symbol)
#define __MACRO_GUARD(symbol) MACRO_CONCAT(symbol ## _at_line_, __LINE__)

//Короткий синоним для MACRO_GUARD
#define __mgrd(symbol) MACRO_GUARD(symbol)


#endif