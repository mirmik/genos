MEMORY
{
	flash  (rx)   : ORIGIN = 0,        LENGTH = 256K
	sram   (rw!x) : ORIGIN = 0x800200, LENGTH = 0xfe00
	eeprom (rw!x) : ORIGIN = 0x810000, LENGTH = 4K
}

OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:6)
ENTRY(__start)

SECTIONS
{
    .text :
    {
		. = ALIGN(4);
		_stext = . ;
    	*(.vectors)
    	KEEP(*(.vectors))
    	*(.init)
    	*(.progmem.gcc*)
    	*(.progmem*)

		. = ALIGN(2);
		*(.trampolines)
    	*(.trampolines*)
		*(.handlers)

     	__ctors_start = . ;
     	KEEP(SORT(*)(.ctors))
    	__ctors_end = . ;
     	_dtors_start = . ;
     	KEEP(SORT(*)(.dtors))
		_dtors_end = . ;
    	
		. = ALIGN(2);
		*(.text)
		*(.text.*)
		_etext = . ;
	} > flash

	_sidata = LOADADDR(.data);
	.data : 
	{
		. = ALIGN(4);
		__data_start = .;        /* create a global symbol at data start */
		*(.data)           /* .data sections */
		*(.data*)          /* .data* sections */
		*(.rodata)
		*(.rodata)
	
		. = ALIGN(4);
		__data_end = .;        /* define a global symbol at data end */
	} >sram AT> flash

	.bss			:
	{
		. = ALIGN(4);
		__bss_start = ABSOLUTE(.);
		*(.bss .bss.*)
		__bss_end = ABSOLUTE(.);
		_heap_start = ABSOLUTE(.);
	} > sram

	.noinit			:
	{
		. = ALIGN(4);
		_snoinit = ABSOLUTE(.);
		*(.noinit*)
		_enoinit = ABSOLUTE(.);
	}	> sram

	.eeprom			:
	{
		. = ALIGN(4);
		_seeprom = ABSOLUTE(.);
		*(.eeprom*)
		_eeeprom = ABSOLUTE(.);
	}	> eeprom
}
