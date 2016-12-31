/*	STM32 Start Label */

#include <asm/irq.h>
#include <string.h>

typedef void (*func_ptr)(void); 

extern unsigned char _data_virtual_address;
extern unsigned char _data_load_address;
extern unsigned char _data_size;

extern unsigned char _sbss;
extern unsigned char _ebss;

extern int main();

extern func_ptr __init_array_start[0], __init_array_end[0];
extern func_ptr __fini_array_start[0], __fini_array_end[0];

void cxx_invoke_init_array(void) {
	for ( func_ptr* func = __init_array_start; func != __init_array_end; func++ )	{
		(*func)();
	}
}

void cxx_invoke_fini_array(void) {
	for ( func_ptr* func = __fini_array_start; func != __fini_array_end; func++ )	{
		(*func)();
	}
}

void __start(void) {
	global_irq_disable();

	/*data section copy*/
	memcpy(&_data_virtual_address, &_data_load_address, (unsigned int) &_data_size);
	
	/*bss section init*/
	unsigned int bss_size = &_ebss - &_sbss;
	memset(&_sbss, '\0', bss_size);

	/*global ctors invoke*/
	cxx_invoke_init_array();
	
	/*main programm start*/
	main();
	
	/*global dtors invoke*/
	cxx_invoke_fini_array();

	/*programm end stub*/
	global_irq_disable();
	for( ; ; );
}