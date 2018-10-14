#ifndef DRIVERS_ATMEL_DATAFLASH_H
#define DRIVERS_ATMEL_DATAFLASH_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <drivers/spi/spi.h>

#include <gxx/debug/dprint.h>

#include <systime/systime.h>
#include <avr/io.h>

/*__flash const df_Info_t df_table [] =
{
   { 512,  264,  9,  (0x3<<2) }, // 1M  AT45DB011
   { 1024, 264,  9,  (0x5<<2) }, // 2M  AT45DB021
   { 2048, 264,  9,  (0x7<<2) }, // 4M  AT45DB041
   { 4096, 264,  9,  (0x9<<2) }, // 8M  AT45DB081
   { 4096, 528,  10, (0xB<<2) }, // 16M AT45DB161
   { 8192, 528,  10, (0xD<<2) }, // 32M AT45DB321
   { 8192, 1056, 11, (0xF<<2) }, // 64M AT45DB642
};*/

/*****************************************************************************
*  Коды комманд для использования с df_PageFunc
******************************************************************************/
#define DF_FLASH_TO_BUF1 		0x53	/* Main memory page to buffer 1 transfer  */
#define DF_FLASH_TO_BUF2 		0x55	/* Main memory page to buffer 2 transfer */
#define DF_BUF1_TO_FLASH_WITH_ERASE   	0x83	/* Buffer 1 to main memory page program with built-in erase */
#define DF_BUF2_TO_FLASH_WITH_ERASE   	0x86	/* Buffer 2 to main memory page program with built-in erase */
#define DF_BUF1_TO_FLASH_WITHOUT_ERASE  0x88	/* Buffer 1 to main memory page program without built-in erase */
#define DF_BUF2_TO_FLASH_WITHOUT_ERASE  0x89	/* Buffer 2 to main memory page program without built-in erase */

/*struct df_info
{
   uint16  pages;       // Количество страниц
   uint16  page_size;   // Размер страницы в байтах
   uint8   page_bit;    // Длина адреса страницы
   uint8   chip_id;     // Код устройства
};*/

struct atmel_dataflash_device 
{
	struct spi_device * spi;

   uint16_t  pages;       // Количество страниц
   uint16_t  page_size;   // Размер страницы в байтах
   uint8_t   page_bit;    // Длина адреса страницы
   uint8_t   chip_id;     // Код устройства

   void* slct;

   constexpr atmel_dataflash_device(struct spi_device * spidev, void* slct, uint16_t pages, uint16_t page_size, uint8_t page_bit, uint8_t chip_id) 
      : spi(spidev), slct(slct), pages(pages), page_size(page_size), page_bit(page_bit), chip_id(chip_id) {}

   void reset() {
      DDRG |= (1<<0);
      
      PORTG &= ~(1<<0);  // сбрасываем dataflsah
      delay(10);          // RESET Pulse Width = 10us
      PORTG |= (1<<0);
      delay(1);           // RESET Recovery Time = 1us
   }

   uint8_t get_status();
   
   void lock() {
      dprln("atmel_dataflash_lock");
      spi->lock();
      spi->select(slct, true);
   }

   void unlock() {
      dprln("atmel_dataflash_unlock");
      spi->select(slct, false);
      spi->unlock();   
   }
};

struct AT45DB011 : public atmel_dataflash_device { AT45DB011(struct spi_device * spidev, void* slct) : atmel_dataflash_device(spidev, slct, 512,  264,  9,  (0x3<<2)) {} };
struct AT45DB021 : public atmel_dataflash_device { AT45DB021(struct spi_device * spidev, void* slct) : atmel_dataflash_device(spidev, slct, 1024, 264,  9,  (0x5<<2)) {} };

//CRIUS AIOP v2
struct AT45DB041 : public atmel_dataflash_device { AT45DB041(struct spi_device * spidev, void* slct) : atmel_dataflash_device(spidev, slct, 2048, 264,  9,  (0x7<<2)) {} };
struct AT45DB081 : public atmel_dataflash_device { AT45DB081(struct spi_device * spidev, void* slct) : atmel_dataflash_device(spidev, slct, 4096, 264,  9,  (0x9<<2)) {} };
struct AT45DB161 : public atmel_dataflash_device { AT45DB161(struct spi_device * spidev, void* slct) : atmel_dataflash_device(spidev, slct, 4096, 528,  10, (0xB<<2)) {} };
struct AT45DB321 : public atmel_dataflash_device { AT45DB321(struct spi_device * spidev, void* slct) : atmel_dataflash_device(spidev, slct, 8192, 528,  10, (0xD<<2)) {} };
struct AT45DB642 : public atmel_dataflash_device { AT45DB642(struct spi_device * spidev, void* slct) : atmel_dataflash_device(spidev, slct, 8192, 1056, 11, (0xF<<2)) {} };

__BEGIN_DECLS

/*static inline struct df_info info_AT45DB011() { struct df_info ret = { 512,  264,  9,  (0x3<<2) }; return ret; }
static inline struct df_info info_AT45DB021() {	struct df_info ret = { 1024, 264,  9,  (0x5<<2) }; return ret; }
static inline struct df_info info_AT45DB041() {	struct df_info ret = { 2048, 264,  9,  (0x7<<2) }; return ret; }
static inline struct df_info info_AT45DB081() {	struct df_info ret = { 4096, 264,  9,  (0x9<<2) }; return ret; }
static inline struct df_info info_AT45DB161() {	struct df_info ret = { 4096, 528,  10, (0xB<<2) }; return ret; }
static inline struct df_info info_AT45DB321() {	struct df_info ret = { 8192, 528,  10, (0xD<<2) }; return ret; }
static inline struct df_info info_AT45DB642() {	struct df_info ret = { 8192, 1056, 11, (0xF<<2) }; return ret; }
*/

__END_DECLS

#endif