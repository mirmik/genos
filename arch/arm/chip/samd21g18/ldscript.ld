/*
	LDScript для STM32:
	256K Flash
	64K RAM
*/

_estack = 0x2000FFFF;    /* end of 64K RAM */
_eheap = _estack;

MEMORY
{
	FLASH 		(rx)  : ORIGIN = 0x08000000, LENGTH = 256K
	RAM 		(xrw) : ORIGIN = 0x20000000, LENGTH = 64K
	memory_b1 	(rx)  : ORIGIN = 0x60000000, LENGTH = 0K
	ccmram	 	(rw)  : ORIGIN = 0x10000000, LENGTH = 0K
}

ENTRY(__start)

SECTIONS
{
	.text			:
	{
		_stext = . ;
		KEEP(*(.vectors))
		
		*(.handlers)

		__init_array_start = .;
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		__init_array_end = .;

		__fini_array_start = .;
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		__fini_array_end = .;

		. = ALIGN(2);
		*(.text)
		*(.text.*)
		_etext = . ;
	} > FLASH

	_eronly = ABSOLUTE(.);

	.rodata :
  	{
    	. = ALIGN(4);
    	*(.rodata)
    	*(.rodata*)
    	. = ALIGN(4);
  	} >FLASH

	.data			:
	{
		_data_virtual_address = .;
		
		_sdata = ABSOLUTE(.);
		KEEP(*(.data))
		*(.data.*)
		KEEP(*(.rodata))
		*(.rodata.*)                
		_edata = ABSOLUTE(.);
	} > RAM AT> FLASH

	_data_size = SIZEOF(.data);
	_data_load_address = LOADADDR(.data);

	.bss			:
	{
		_sbss = ABSOLUTE(.);
		*(.bss .bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
		_ebss = ABSOLUTE(.);
		_heap_start = ABSOLUTE(.);
	} > RAM

	__exidx_start = .;
    .ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } >RAM
    __exidx_end = .;
}