#ifndef DRIVERS_ATMEL_DATAFLASH_H
#define DRIVERS_ATMEL_DATAFLASH_H

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


struct df_info
{
   uint16  pages;       // Количество страниц
   uint16  page_size;   // Размер страницы в байтах
   uint8   page_bit;    // Длина адреса страницы
   uint8   chip_id;     // Код устройства
};

struct atmel_dataflash_device 
{
	struct spi_device * spi;
	struct df_info info;
}

__BEGIN_DECLS

static inline struct df_info info_AT45DB011() { struct df_info ret = { 512,  264,  9,  (0x3<<2) }; return ret; }
static inline struct df_info info_AT45DB021() {	struct df_info ret = { 1024, 264,  9,  (0x5<<2) }; return ret; }
static inline struct df_info info_AT45DB041() {	struct df_info ret = { 2048, 264,  9,  (0x7<<2) }; return ret; }
static inline struct df_info info_AT45DB081() {	struct df_info ret = { 4096, 264,  9,  (0x9<<2) }; return ret; }
static inline struct df_info info_AT45DB161() {	struct df_info ret = { 4096, 528,  10, (0xB<<2) }; return ret; }
static inline struct df_info info_AT45DB321() {	struct df_info ret = { 8192, 528,  10, (0xD<<2) }; return ret; }
static inline struct df_info info_AT45DB642() {	struct df_info ret = { 8192, 1056, 11, (0xF<<2) }; return ret; }

__END_DECLS

#endif