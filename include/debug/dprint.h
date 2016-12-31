#ifndef GENOS_DEBUG_PRINT
#define GENOS_DEBUG_PRINT
	
#include <inttypes.h> 
#include <compiler.h>

#ifdef __cplusplus
#include "gxx/buffer.h"
#endif

/*	
	2012-2015 Mirmik
	Библиотека низкоуровневого отладочного вывода.
	
	Библиотека предназначена для облегчения низкоуровневой отладки.
	Функции не имеют внешних зависимостей (кроме определения типов 
	и аппаратнозависимой инициализации и функции вывода символа), 
	линкуются по стандарту "С", что позволяет
	вызывать их в том числе с использованием ассемблера. 
	
	Библиотека призвана помочь в создании плацдарма на неосвоенном железе,
	а так же при отладке средств ввода-вывода.
	
	Для подключения библиотеки необходимо реализовать платформозависимую функцию
	debug_putchar, выводящую один символ char и 
	блокирующую процессор на время проведения этой операции.
*/

__BEGIN_DECLS
//Инициализация вывода
//void debug_print_init(); - в platform

//Платформозависимый !блокирующий процессор! вывод char
void debug_putchar(char c);

//Вывод строки указанной длины.
void debug_write(const void* c, int i);
//Вывод интов, согласно типам. Возможен вывод в системах dec, hex, bin.
 void debug_printhex_uint4(uint8_t b);
 void debug_printhex_uint8 (uint8_t b);
 void debug_printhex_uint64(uint64_t a);
 void debug_printhex_uint32(uint32_t a);

 #define debug_printhex_int4(a) debug_printhex_uint4(a)
 #define debug_printhex_int8(a) debug_printhex_uint8(a)
 #define debug_printhex_int16(a) debug_printhex_uint16(a) 
 #define debug_printhex_int32(a) debug_printhex_uint32(a)

 void debug_printhex_ptr(void* a);
 void debug_printhex_uint16(uint16_t a);
 void debug_printbin_uint4 (uint8_t b);
 void debug_printbin_uint8 (uint8_t b);
 void debug_printbin_uint64(uint64_t a);
 void debug_printbin_uint32(uint32_t a);
 void debug_printbin_uint16(uint16_t a);
 void debug_printdec_uint8 (uint8_t b);
 void debug_printdec_uint64(uint64_t a);
 void debug_printdec_uint32(uint32_t a);
 void debug_printdec_uint16(uint16_t a);
 void debug_printdec_int8 (int8_t b);
 void debug_printdec_int64(int64_t a);
 void debug_printdec_int32(int32_t a);
 void debug_printdec_int16(int16_t a);

 void debug_printdec_float(float a, int prec);
 void debug_printdec_double(double a, int prec);

//strlen для внутреннего использования
 int debug_strlen(const char *c);

//Вывод нульформатированной строки
 void debug_print(const char *c);
 void debug_print_line();

//Вывод дампа памяти. size - размер в байтах.
 void debug_print_dump(const void* address, uint16_t size);
 void debug_print_dump_ascii(const void* address, uint16_t size);
 
 void debug_print_dump_simple(void* address, uint16_t size);

//Вывод логического значения.
 void debug_print_bool(uint8_t b);
 void debug_print_bool_num(uint8_t b);

//Вывод float одинарной точности. //TODO дополнить
//void debug_printbin_spf(sp_float a);

//Проверка функций библиотеки debug_print.
void debug_print_test();

//Тесты для проверки ассемблерной линковки.
 void dasm_test();
 //void dasm_uint8_dpr1(uint8_t a);
 //void dasm_uint8_dpr2(uint8_t a, uint8_t b);
 //void dasm_uint8_dpr3(uint8_t a, uint8_t b, uint8_t c);
 //void dasm_uint8_dpr4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
 void dasm_printhex_uint16(uint16_t a);
 void dasm_uint16_dpr2(uint16_t a, uint16_t b);
 void dasm_uint16_dpr4(uint16_t a, uint16_t b, uint16_t c, uint16_t d);
 uint16_t dasm_uint16_ret();
 uint64_t dasm_uint64_ret();

__END_DECLS

#define dln() {debug_putchar('\r');debug_putchar('\n');}

#ifdef __cplusplus

void dprln();

void dpr(char obj);
void dpr(char* obj);
void dpr(const char* obj);

//template<> void dpr(genos::charptr obj) 
//{ 
//	debug_write(obj.to_buf(),obj.length()); 
//};

void dpr(uint8_t obj);
void dpr(uint16_t obj);
void dpr(uint32_t obj);
void dpr(uint64_t obj);

void dpr(int8_t obj);
void dpr(int16_t obj);
void dpr(int32_t obj);
void dpr(int64_t obj);

void dpr(bool obj);

void dpr(float obj);
void dpr(double obj);

template <typename Buffer>
void dpr(const Buffer& obj) {
	debug_write(obj.data(), obj.size());
}

void dprbin(uint8_t obj);
void dprbin(uint16_t obj);
void dprbin(uint32_t obj);
void dprbin(uint64_t obj);

void dprbin(int8_t obj);
void dprbin(int16_t obj);
void dprbin(int32_t obj);
void dprbin(int64_t obj);

template <typename Buffer>
void dprhex(const Buffer& obj) {
	debug_print_dump(obj.data(), obj.size());
}

void dprhex(const char* obj);
void dprhex(char obj);

void dprhex(uint8_t obj);
void dprhex(uint16_t obj);
void dprhex(uint32_t obj);
void dprhex(uint64_t obj);

void dprhex(int8_t obj);
void dprhex(int16_t obj);
void dprhex(int32_t obj);
void dprhex(int64_t obj);

//template<> void dprhex(float obj);
//template<> void dprhex(double obj);
//void dpr(gxx::string obj);

void dpr_dump(void* obj, uint32_t size);
void dpr_dump_ascii(void* obj, uint32_t size);

template<typename T> void dprptr(const T*& obj) 
	{ debug_printhex_ptr((void*)obj); };

template<typename T> void dprln(const T& obj) 
	{ dpr(obj); debug_write("\r\n", 2); };

template<typename T> void dprbinln(const T& obj) 
	{ dprbin(obj); debug_write("\r\n", 2); };

template<typename T> void dprhexln(const T& obj) 
	{ dprhex(obj); debug_write("\r\n", 2); };

template<typename T> void dprptrln(const T& obj) 
	{ dprptr(obj); debug_write("\r\n", 2); };

void dprtab();
void dprtab(uint8_t obj);

#endif //__cplusplus

#endif