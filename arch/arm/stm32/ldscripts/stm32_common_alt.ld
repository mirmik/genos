/* Memory layout for basic configuration:

	FLASH       .vectors
	FLASH       .text
	FLASH       .data

	RAM         .data
	RAM         .bss
	RAM         .heap
	RAM         .stack
*/

_minimum_heap_size = 0;      /* required amount of heap  */
_minimum_stack_size = 0x400; /* required amount of stack */

_estack = ORIGIN(RAM) + LENGTH(RAM) - 4;

ENTRY(_start)

/* define output sections */
SECTIONS
{
	/* The startup code goes first into FLASH */
	.vectors :
	{
		. = ALIGN(4);
		KEEP(*(.vectors)) /* Startup code */
		. = ALIGN(4);
	} >FLASH

	/* The program code and other data goes into FLASH */.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.text*)
		*(.glue_7)         /* glue arm to thumb code */
		*(.glue_7t)        /* glue thumb to arm code */
		*(.eh_frame)

		KEEP (*(.init))
		KEEP (*(.fini))

		. = ALIGN(4);
		_etext = .;
	} >FLASH


 	/* Constant data goes into FLASH */
  	.rodata :
  	{
    	. = ALIGN(4);
    	*(.rodata)
    	*(.rodata*)
    	. = ALIGN(4);
  	} >FLASH

	.preinit_array     :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} >FLASH

	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	} >FLASH

	.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} >FLASH

	/* used by the startup to initialize data */
	_sidata = LOADADDR(.data);
	
	/* Initialized data sections goes into RAM, load LMA copy after code */
	.data : 
	{
		. = ALIGN(4);
		_sdata = .;        /* create a global symbol at data start */
		*(.data)           /* .data sections */
		*(.data*)          /* .data* sections */
	
		. = ALIGN(4);
		_edata = .;        /* define a global symbol at data end */
	} >RAM AT> FLASH

	/* Uninitialized data section */
	. = ALIGN(4);
	.bss :
	{
		/* This is used by the startup in order to initialize the .bss secion */
		_sbss = .;         /* define a global symbol at bss start */
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss*)
		*(COMMON)
	
		. = ALIGN(4);
		_ebss = .;         /* define a global symbol at bss end */
		__bss_end__ = _ebss;
	} >RAM


	/* this is to define the start of the heap, and make sure we have a minimum size */
	.heap :
	{
		. = ALIGN(4);
		_heap_start = .;
		. = . + _minimum_heap_size;
		. = ALIGN(4);
	} >RAM

	/* this just checks there is enough RAM for the stack */
	.stack :
	{
		. = ALIGN(4);
		. = . + _minimum_stack_size;
		. = ALIGN(4);
	} >RAM

	.ARM.exidx : {
    	__exidx_start = .;
    	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
    	__exidx_end = .;
	} >RAM

	/* Remove information from the standard libraries */
	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
  	}

	.ARM.attributes 0 : { *(.ARM.attributes) }
}