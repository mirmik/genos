/*OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)*/
ENTRY(__start)

/* Memory Spaces Definitions */
MEMORY
{
  	FLASH (rx) : ORIGIN = 0x00000000+0x4000, LENGTH = 0x00040000-0x4000
  	RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

_estack = 0x20007FFF;    /* end of 64K RAM */
/*_eheap = _estack;*/

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
  	} > FLASH

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