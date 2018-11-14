#ifndef DRIVERS_ATMEL_DATAFLASH_H
#define DRIVERS_ATMEL_DATAFLASH_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <drivers/spi/spi.h>
#include <drivers/gpio/pin.h>

#include <gxx/debug/dprint.h>

#include <systime/systime.h>
#include <avr/io.h>

/**
 * @detail slct и rst работают в реверсивной логике ???
 *
 */


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


//------------------------------------------------------------------------------------
//  Коды комманд для внутреннего использования
//------------------------------------------------------------------------------------
#define FlashPageRead         0x52  // Main memory page read
#define ContArrayRead         0x68  // Continuous Array Read (Note : Only A/B/D-parts supported)
#define StatusReg       0x57  // Status register
#define Buf1Read        0x54  // Buffer 1 read
#define Buf2Read        0x56  // Buffer 2 read
#define Buf1Write       0x84  // Buffer 1 write
#define Buf2Write       0x87  // Buffer 2 write

#define DF_STATUS_READY 0x80

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

   int page_in_buffer1 = -1;
   int page_in_buffer2 = -1;

   gpio_pin* slct;
   gpio_pin* rst;

   constexpr atmel_dataflash_device(struct spi_device * spidev, gpio_pin* slct, gpio_pin* rst, uint16_t pages, uint16_t page_size, uint8_t page_bit, uint8_t chip_id) 
      : spi(spidev), slct(slct), rst(rst), pages(pages), page_size(page_size), page_bit(page_bit), chip_id(chip_id) {}

   void reset() {
      slct -> settings(GPIO_MODE_OUTPUT);
      rst  -> settings(GPIO_MODE_OUTPUT);

      slct -> set(1);

      rst -> set(0);
      delay(10);          // RESET Pulse Width = 10us
      rst -> set(1);
      delay(1);           // RESET Recovery Time = 1us
   }

   uint8_t get_status();
   
   void flashpage_to_buffer1(int pagenum) 
   {
      lock();
      pagefunc(DF_FLASH_TO_BUF1, pagenum);
      unlock();
   }

   void buffer1_to_flashpage(int pagenum) 
   {
      lock();
      pagefunc(DF_BUF1_TO_FLASH_WITH_ERASE, pagenum);
      unlock();
   }

   void flashpage_to_buffer2(int pagenum) 
   {
      lock();
      pagefunc(DF_FLASH_TO_BUF2, pagenum);
      unlock();
   }

   void buffer2_to_flashpage(int pagenum) 
   {
      lock();
      pagefunc(DF_BUF2_TO_FLASH_WITH_ERASE, pagenum);
      unlock();
   }

   //------------------------------------------------------------------------------------
   //  Прочитать блок данных непосредственно из Flash - памяти
   // Параметры: PageAdr    -> Номер страницы, с которой начинать чтение
   //                  Addr       -> Адрес в странице, с которого начинать чтение
   //                  Count      -> Количество байт, которое необходимо прочитать
   //          *BufferPtr -> Адрес буффера в который заносить данные
   //------------------------------------------------------------------------------------
   void flashread( uint16_t page, uint16_t off, uint8_t *data, uint16_t size )
   {
      lock();
   
      spi->exbyte(ContArrayRead);
      spi->exbyte((unsigned char)(page >> (16 - page_bit)));
      spi->exbyte((unsigned char)((page << (page_bit - 8))+ (off>>8)));
      spi->exbyte((unsigned char)(off));
      spi->exbyte(0x00);
      spi->exbyte(0x00);
      spi->exbyte(0x00);
      spi->exbyte(0x00);
   
      for( uint16_t i=0; i < size; i++ )
      {
         *data++ = spi->exbyte(0x00);
      }
   
      unlock();
   }

   uint8_t is_ready()
   {
      return get_status() &  DF_STATUS_READY;
   }

   //------------------------------------------------------------------------------------
   //  Манипуляции со страницами
   // Параметры: PageСmd  -> Команда
   //                  PageAdr  -> Номер страницы
   //------------------------------------------------------------------------------------
   void pagefunc( uint8_t PageCmd, uint16_t PageAdr )
   {
      uint8_t sts;

      lock(); 

      sts = spi->exbyte(PageCmd);
      sts = spi->exbyte((uint8_t)(PageAdr >> (16 - page_bit)));      
      sts = spi->exbyte((uint8_t)(PageAdr << (page_bit - 8)));
      sts = spi->exbyte(0x00);
   
      while(!( get_status() & DF_STATUS_READY )); // Ожидаем завершения операции

      unlock();
   }


   //------------------------------------------------------------------------------------
   //  Записывает блок данных в SRAM-буффер dataflash
   // Параметры: BufferNo   -> Номер буффера (1 или 2)
   //                  Addr       -> Адрес в буффере с которого начинать чтение
   //                  Count      -> Количество байт, которое необходимо записать
   //          *BufferPtr -> Буффер, данные из которого нужно записать
   //------------------------------------------------------------------------------------
   void write_to_buffer( uint8_t bufno, uint16_t off, const void *_data, uint16_t size )
   {
      dprln("write_to_buffer");

      uint8_t c;
      uint8_t off0 = off>>8;
      uint8_t off1 = off;
      const uint8_t * data = (const uint8_t *) _data;

      dprln(off0);
      dprln(off1);

      lock();
   
      if ( bufno == 1 ) { spi->exbyte(Buf1Write); }       // буффер 1
      else  if ( bufno == 2 ) { spi->exbyte(Buf2Write); }  // буффер 2
      else { unlock(); return; }                         // ошиблись с номером буффера
   
      spi->exbyte(0x00);
      spi->exbyte(off0);
      spi->exbyte(off1);
      for( uint16_t i = 0; i < size; i++)
      {
         spi->exbyte(*data++);
      }
      
      unlock();
   }

   //------------------------------------------------------------------------------------
   //  Читает блок данных из SRAM-буффера dataflash
   // Параметры: BufferNo   -> Номер буффера (1 или 2)
   //                  Addr       -> Адрес в буффере с которого начинать чтение
   //                  Count      -> Количество байт, которое необходимо прочитать
   //          *BufferPtr -> Адрес буффера в который заносить данные
   //------------------------------------------------------------------------------------
   void read_from_buffer( uint8_t bufno, uint16_t off, uint8_t *data, uint16_t size )
   {
      lock();
   
      if ( bufno == 1 ) { spi->exbyte(Buf1Read); }       // буффер 1
      else  if ( bufno == 2 ) { spi->exbyte(Buf2Read); }  // буффер 2
      else { unlock(); return; }                        // ошиблись с номером буффера
   
      spi->exbyte(0x00);
      spi->exbyte((unsigned char)(off>>8));
      spi->exbyte((unsigned char)(off));
      spi->exbyte(0x00);
   
      for( uint16_t i=0; i < size; i++)
      {
         *data++ = spi->exbyte(0x00);
      }
   
      unlock();
   }

   void lock() {
      spi->lock();
      slct->set(0);
   }

   void unlock() {
      slct->set(1);
      spi->unlock();   
   }
};

struct AT45DB011 : public atmel_dataflash_device { AT45DB011(struct spi_device * spidev, gpio_pin* slct, gpio_pin* rst) : atmel_dataflash_device(spidev, slct, rst, 512,  264,  9,  (0x3<<2)) {} };
struct AT45DB021 : public atmel_dataflash_device { AT45DB021(struct spi_device * spidev, gpio_pin* slct, gpio_pin* rst) : atmel_dataflash_device(spidev, slct, rst, 1024, 264,  9,  (0x5<<2)) {} };
struct AT45DB041 : public atmel_dataflash_device { AT45DB041(struct spi_device * spidev, gpio_pin* slct, gpio_pin* rst) : atmel_dataflash_device(spidev, slct, rst, 2048, 264,  9,  (0x7<<2)) {} };
struct AT45DB081 : public atmel_dataflash_device { AT45DB081(struct spi_device * spidev, gpio_pin* slct, gpio_pin* rst) : atmel_dataflash_device(spidev, slct, rst, 4096, 264,  9,  (0x9<<2)) {} };
struct AT45DB161 : public atmel_dataflash_device { AT45DB161(struct spi_device * spidev, gpio_pin* slct, gpio_pin* rst) : atmel_dataflash_device(spidev, slct, rst, 4096, 528,  10, (0xB<<2)) {} }; ///< CRIUS AIOP v2
struct AT45DB321 : public atmel_dataflash_device { AT45DB321(struct spi_device * spidev, gpio_pin* slct, gpio_pin* rst) : atmel_dataflash_device(spidev, slct, rst, 8192, 528,  10, (0xD<<2)) {} };
struct AT45DB642 : public atmel_dataflash_device { AT45DB642(struct spi_device * spidev, gpio_pin* slct, gpio_pin* rst) : atmel_dataflash_device(spidev, slct, rst, 8192, 1056, 11, (0xF<<2)) {} };

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