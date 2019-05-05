/* Memory Regions *************************************************************/
/* ------ ------ ------+------- -- ------ -- ------ --- ------ ----+------- ---
 *         FLASH       | REGISTERS I/O       EXT I/O    ISRAM      | EEPROM
 *                     |           REGISTERS REGISTERS             |
 * ------- ------ -----+------- -- ------ -- ------ --- ------ ----+------- ---
 *         0x0000 256Kb| 0x0000 32 0x0020 64 0x0060 416 0x0200 8Kb | 0x0000 4Kb
 * ------- ------ -----+------- -- ------ -- ------ --- ------ ----+------- ---
 * *Memory configuration A
 */

__TEXT_REGION_LENGTH__ = DEFINED(__TEXT_REGION_LENGTH__) ? __TEXT_REGION_LENGTH__ : 128K;
__DATA_REGION_LENGTH__ = DEFINED(__DATA_REGION_LENGTH__) ? __DATA_REGION_LENGTH__ : 0xffa0;
__EEPROM_REGION_LENGTH__ = DEFINED(__EEPROM_REGION_LENGTH__) ? __EEPROM_REGION_LENGTH__ : 64K;
__FUSE_REGION_LENGTH__ = DEFINED(__FUSE_REGION_LENGTH__) ? __FUSE_REGION_LENGTH__ : 1K;
__LOCK_REGION_LENGTH__ = DEFINED(__LOCK_REGION_LENGTH__) ? __LOCK_REGION_LENGTH__ : 1K;
__SIGNATURE_REGION_LENGTH__ = DEFINED(__SIGNATURE_REGION_LENGTH__) ? __SIGNATURE_REGION_LENGTH__ : 1K;
__USER_SIGNATURE_REGION_LENGTH__ = DEFINED(__USER_SIGNATURE_REGION_LENGTH__) ? __USER_SIGNATURE_REGION_LENGTH__ : 1K;

MEMORY
{
  flash   (rx)   : ORIGIN = 0, LENGTH = __TEXT_REGION_LENGTH__
  sram   (rw!x) : ORIGIN = 0x800060, LENGTH = __DATA_REGION_LENGTH__
  eeprom (rw!x) : ORIGIN = 0x810000, LENGTH = __EEPROM_REGION_LENGTH__
  fuse      (rw!x) : ORIGIN = 0x820000, LENGTH = __FUSE_REGION_LENGTH__
  lock      (rw!x) : ORIGIN = 0x830000, LENGTH = __LOCK_REGION_LENGTH__
  signature (rw!x) : ORIGIN = 0x840000, LENGTH = __SIGNATURE_REGION_LENGTH__
  user_signatures (rw!x) : ORIGIN = 0x850000, LENGTH = __USER_SIGNATURE_REGION_LENGTH__
}

OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:5)
ENTRY(__start)

SECTIONS
{
	.text			:
	{
		_stext = . ;
		KEEP(*(.vectors))
    	*(.progmem.gcc*)
    	*(.progmem*)


		. = ALIGN(2);
		*(.trampolines)
    	*(.trampolines*)
    	*(.init)
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

	.data			:
	{
		__data_start = ABSOLUTE(.);
		KEEP(*(.data))
		*(.data.*)
		KEEP(*(.rodata))
		*(.rodata.*)                
		__data_end = ABSOLUTE(.);
	} > sram AT > flash

	.bss			:
	{
		__bss_start = ABSOLUTE(.);
		*(.bss .bss.*)
		__bss_end = ABSOLUTE(.);
		_heap_start = ABSOLUTE(.);
	} > sram

	/* Global data not cleared after reset.	*/

	.noinit			:
	{
		_snoinit = ABSOLUTE(.);
		*(.noinit*)
		_enoinit = ABSOLUTE(.);
	}	> sram

	.eeprom			:
	{
		_seeprom = ABSOLUTE(.);
		*(.eeprom*)
		_eeeprom = ABSOLUTE(.);
	}	> eeprom
}
