#include <drivers/bdev/atmel_dataflash.h>

//------------------------------------------------------------------------------------
//  Коды комманд для внутреннего использования
//------------------------------------------------------------------------------------
#define FlashPageRead			0x52	// Main memory page read
#define ContArrayRead			0x68	// Continuous Array Read (Note : Only A/B/D-parts supported)
#define StatusReg			0x57	// Status register
#define Buf1Read			0x54	// Buffer 1 read
#define Buf2Read			0x56	// Buffer 2 read
#define Buf1Write			0x84	// Buffer 1 write
#define Buf2Write			0x87	// Buffer 2 write

#define DF_STATUS_READY 0x80
