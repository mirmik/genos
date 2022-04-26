/*	STM32 Start Label */

#include <periph/map.h>
#include <asm/irq.h>
#include <string.h>

typedef void (*func_ptr)(void);

extern unsigned char _sdata;
extern unsigned char _edata;
extern unsigned char _sidata;

extern unsigned char _sbss;
extern unsigned char _ebss;

extern int main();

extern func_ptr __init_array_start[0], __init_array_end[0];
extern func_ptr __fini_array_start[0], __fini_array_end[0];

void cxx_invoke_init_array(void)
{
	for ( func_ptr* func = __init_array_start; func != __init_array_end; func++ )
	{
		(*func)();
	}
}

void cxx_invoke_fini_array(void)
{
	for ( func_ptr* func = __fini_array_start; func != __fini_array_end; func++ )
	{
		(*func)();
	}
}

void __start(void)
{
	irqs_disable();

	/*data section copy*/
	memcpy(&_sdata, &_sidata, (unsigned int) (&_edata - &_sdata));

	/*bss section init*/
	unsigned int bss_size = &_ebss - &_sbss;
	memset(&_sbss, '\0', bss_size);

#if (__FPU_USED == 1)
	// enable FPU if available and used
	SCB->CPACR |= ((3UL << 10 * 2) |     // set CP10 Full Access
	               (3UL << 11 * 2) );    // set CP11 Full Access
#endif

	// global ctors invoke
	cxx_invoke_init_array();

	// main programm start
	main();

	// global dtors invoke
	cxx_invoke_fini_array();

	// programm end stub
	irqs_disable();
	for ( ; ; );
}