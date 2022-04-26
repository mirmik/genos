/*************************************************************************************
*
*  Драйвер Atmel DataFlash
*
*  Совместимо с Strict ANSI C, MISRA-C 2004
*
*  by:      BSVi
*  version: 1.00
*
*************************************************************************************/

#include "dataflash.h"

#define DF_MISO_PB   BIT6
#define DF_MOSI_PB   BIT5
#define DF_SCK_PB    BIT7
#define DF_SS_PB     BIT3
#define DF_RESET_PB  BIT4

#define SELECT()  PORTB &= ~DF_SS_PB;  // Ножка SS
#define DESELECT() PORTB |= DF_SS_PB;

//------------------------------------------------------------------------------------
//  Определение типа dataflash
//------------------------------------------------------------------------------------
#ifdef DF_AUTODETECT_FEATURES


df_Info_t df_Info;

__flash const df_Info_t df_table [] =
{
   { 512,  264,  9,  (0x3<<2) }, // 1M  AT45DB011
   { 1024, 264,  9,  (0x5<<2) }, // 2M  AT45DB021
   { 2048, 264,  9,  (0x7<<2) }, // 4M  AT45DB041
   { 4096, 264,  9,  (0x9<<2) }, // 8M  AT45DB081
   { 4096, 528,  10, (0xB<<2) }, // 16M AT45DB161
   { 8192, 528,  10, (0xD<<2) }, // 32M AT45DB321
   { 8192, 1056, 11, (0xF<<2) }, // 64M AT45DB642
};

#else // DF_AUTODETECT_FEATURES

#error "Автоопределение запрещено."

// Скопируйте сюда соответствующую строчку из таблицы выше и
// закомменитируйте сообщение об ошибке
static const df_Info_t DfInfo = { 2048, 264,  9,  (0x7<<2) };

#endif // DF_AUTODETECT_FEATURES


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


//------------------------------------------------------------------------------------
//  Внутренние функции
//------------------------------------------------------------------------------------
static Result df_DetectParams();
static uint8 DF_SPI_RW( uint8 tx );
static uint8 df_GetStatus();


//------------------------------------------------------------------------------------
//  Инициализирует SPI для работы с dataflash
//------------------------------------------------------------------------------------
Result df_Init()
{
   SPCR = 0; // Отключаем SPI чтобы сконфигурировать направление ножек
   DESELECT();

   DDRB &= ~(DF_MISO_PB);

   DDRB |= DF_MOSI_PB + DF_SCK_PB + DF_RESET_PB + DF_SS_PB;    /* MOSI, SS, RESET, SCK - выходы */

   PORTB &= ~DF_RESET_PB;  // сбрасываем dataflsah
   _delay_us(10);          // RESET Pulse Width = 10us
   PORTB |= DF_RESET_PB;
   _delay_us(1);           // RESET Recovery Time = 1us

   // максимальная частота dataflash 15МГц, значительно больше чем может выдать контроллер
   SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPHA) | (1<<CPOL);	// Enable SPI in Master mode, mode 3
   SPSR = (1<<SPI2X);
   return df_DetectParams();
}


//------------------------------------------------------------------------------------
//  Передает параметер и возвращает принятый байт
//------------------------------------------------------------------------------------
static uint8 DF_SPI_RW( uint8 tx )
{
   uint8 rx;

   SPDR = tx;
   while(!(SPSR & 0x80));
   rx = SPDR;
   return rx;
}		

//------------------------------------------------------------------------------------
//  Проверяет, готова ли память
//------------------------------------------------------------------------------------
uint8 df_isReady()
{
   return df_GetStatus() &  DF_STATUS_READY;
}


//------------------------------------------------------------------------------------
//  Читает байт состояния Dataflash. О содержимом этого байта можно узнать в
//  разделе STATUS REGISTER READ.
//------------------------------------------------------------------------------------
static uint8 df_GetStatus()
{
   uint8 status;

   SELECT();
   status = DF_SPI_RW(StatusReg);
   status = DF_SPI_RW(0x00);
   DESELECT();

   return status;
}

//------------------------------------------------------------------------------------
//  Обнаруживает тип dataflash
//------------------------------------------------------------------------------------
static Result df_DetectParams()
{
#ifdef DF_AUTODETECT_FEATURES
   uint8 chip_id;
   chip_id = df_GetStatus() & 0x3C;

   for (uint8 i=0; i<ARRAY_SIZE(df_table); i++)
   {
      if ( df_table[i].chip_id == chip_id )
      {
         df_Info = df_table[i];
         return S_OK;
      }
   }
   return E_ERR;
#else // DF_AUTODETECT_FEATURES
   return S_OK;
#endif // DF_AUTODETECT_FEATURES
}


//------------------------------------------------------------------------------------
//  Читает один байт из SRAM-буффера dataflash
//	Параметры: BufferNo -> Номер буффера (1 или 2)
//                  Addr  -> Адрес в буффере
//------------------------------------------------------------------------------------
uint8 df_GetChar(uint8 BufferNo, uint16 Addr )
{
   uint8 data;

   SELECT();

   if ( BufferNo == 1 )	{ DF_SPI_RW(Buf1Read); }       // буффер 1
   else	if ( BufferNo == 2 ) { DF_SPI_RW(Buf2Read); }  // буффер 2
   else { DESELECT(); return 0; }                      // ошиблись с номером буффера

   DF_SPI_RW(0x00);              // цикл чтения описан в даташите
   DF_SPI_RW((uint8)(Addr>>8));
   DF_SPI_RW((uint8)(Addr));
   DF_SPI_RW(0x00);
   data = DF_SPI_RW(0x00);

   DESELECT();

   return data;
}



//------------------------------------------------------------------------------------
//  Читает блок данных из SRAM-буффера dataflash
//	Параметры: BufferNo   -> Номер буффера (1 или 2)
//                  Addr       -> Адрес в буффере с которого начинать чтение
//                  Count      -> Количество байт, которое необходимо прочитать
//  		   *BufferPtr -> Адрес буффера в который заносить данные
//------------------------------------------------------------------------------------
void df_GetStr( uint8 BufferNo, uint16 Addr, uint16 Count, uint8 *BufferPtr )
{
   SELECT();

   if ( BufferNo == 1 )	{ DF_SPI_RW(Buf1Read); }       // буффер 1
   else	if ( BufferNo == 2 ) { DF_SPI_RW(Buf2Read); }  // буффер 2
   else { DESELECT(); return; }                        // ошиблись с номером буффера

   DF_SPI_RW(0x00);
   DF_SPI_RW((unsigned char)(Addr>>8));
   DF_SPI_RW((unsigned char)(Addr));
   DF_SPI_RW(0x00);

   for( uint16 i=0; i<Count; i++)
   {
      *(BufferPtr) = DF_SPI_RW(0x00);
      BufferPtr++;
   }

   DESELECT();
}


//------------------------------------------------------------------------------------
//  Записывает байт в SRAM-буффер dataflash
//	Параметры: BufferNo   -> Номер буффера (1 или 2)
//                  Addr       -> Адрес в буффере с которого начинать чтение
//                  Data       -> Байт который нужно записать
//------------------------------------------------------------------------------------
void df_WriteByte( uint8 BufferNo, uint16 Addr, uint8 Data )
{
   SELECT();

   if ( BufferNo == 1 )	{ DF_SPI_RW(Buf1Write); }       // буффер 1
   else	if ( BufferNo == 2 ) { DF_SPI_RW(Buf2Write); }  // буффер 2
   else { DESELECT(); return; }                         // ошиблись с номером буффера

   DF_SPI_RW(0x00);
   DF_SPI_RW((unsigned char)(Addr>>8));
   DF_SPI_RW((unsigned char)(Addr));
   DF_SPI_RW(Data);

   DESELECT();
}


//------------------------------------------------------------------------------------
//  Записывает блок данных в SRAM-буффер dataflash
//	Параметры: BufferNo   -> Номер буффера (1 или 2)
//                  Addr       -> Адрес в буффере с которого начинать чтение
//                  Count      -> Количество байт, которое необходимо записать
//  		   *BufferPtr -> Буффер, данные из которого нужно записать
//------------------------------------------------------------------------------------
void df_WriteStr( uint8 BufferNo, uint16 Addr, uint16 Count, uint8 *BufferPtr )
{
   SELECT();

   if ( BufferNo == 1 )	{ DF_SPI_RW(Buf1Write); }       // буффер 1
   else	if ( BufferNo == 2 ) { DF_SPI_RW(Buf2Write); }  // буффер 2
   else { DESELECT(); return; }                         // ошиблись с номером буффера

   DF_SPI_RW(0x00);
   DF_SPI_RW((unsigned char)(Addr>>8));
   DF_SPI_RW((unsigned char)(Addr));
   for( uint16 i=0; i<Count; i++)
   {
      DF_SPI_RW(*(BufferPtr));
      BufferPtr++;
   }


   DESELECT();
}


//------------------------------------------------------------------------------------
//  Манипуляции со страницами
//	Параметры: PageСmd  -> Команда
//                  PageAdr  -> Номер страницы
//------------------------------------------------------------------------------------
void df_PageFunc( uint8 PageCmd, uint16 PageAdr )
{
   SELECT();

   DF_SPI_RW(PageCmd);
   DF_SPI_RW((uint8)(PageAdr >> (16 - df_Info.page_bit)));
   DF_SPI_RW((uint8)(PageAdr << (df_Info.page_bit - 8)));
   DF_SPI_RW(0x00);

   DESELECT();

   while(!( df_GetStatus() & DF_STATUS_READY )); // Ожидаем завершения операции
}

//------------------------------------------------------------------------------------
//  Прочитать блок данных непосредственно из Flash - памяти
//	Параметры: PageAdr    -> Номер страницы, с которой начинать чтение
//                  Addr       -> Адрес в странице, с которого начинать чтение
//                  Count      -> Количество байт, которое необходимо прочитать
//  		   *BufferPtr -> Адрес буффера в который заносить данные
//------------------------------------------------------------------------------------
void df_FlashRead( uint16 PageAdr, uint16 Addr, uint16 Count, uint8 *BufferPtr )
{
   SELECT();

   DF_SPI_RW(ContArrayRead);
   DF_SPI_RW((unsigned char)(PageAdr >> (16 - df_Info.page_bit)));
   DF_SPI_RW((unsigned char)((PageAdr << (df_Info.page_bit - 8))+ (Addr>>8)));
   DF_SPI_RW((unsigned char)(Addr));
   DF_SPI_RW(0x00);
   DF_SPI_RW(0x00);
   DF_SPI_RW(0x00);
   DF_SPI_RW(0x00);

   for( uint16 i=0; i < Count; i++ )
   {
      *(BufferPtr) = DF_SPI_RW(0x00);
      BufferPtr++;
   }

   DESELECT();
}

/*****************************************************************************/
/*****************************************************************************/
/*             Надстройка для удобного чтения и записи информации            */
/*****************************************************************************/
/*****************************************************************************/
#ifdef DF_ENABLE_LINEAR_SPACE
uint16 df_LinearSpace::page_in_buffer1 = 0xffff;

//------------------------------------------------------------------------------------
//  Переводит указатели на начало памяти
//------------------------------------------------------------------------------------
void df_LinearSpace::GoToZero()
{
   page = 0;
   address = 0;
}

//------------------------------------------------------------------------------------
//  Эта функция возвращает количество байт до конца dataflash.
//  Остальные функции не производят проверку на пересечение границы памяти.
//  Программа пользователя должна производить такую проверку.
//------------------------------------------------------------------------------------
uint32 df_LinearSpace::SpaceToEnd()
{
   return (uint32)df_Info.page_size*(df_Info.pages - page) - (uint32)address;
}

//------------------------------------------------------------------------------------
//  Считать данные
//	Параметры: count      -> Количество байт, которое необходимо прочитать
//  		   *buffer    -> Адрес буффера в который заносить данные
//------------------------------------------------------------------------------------
void df_LinearSpace::Read( uint8 *buffer, uint16 count )
{
   df_FlashRead( page, address, count, buffer );
   address += count;

   while( address >= df_Info.page_size )
   {
      address -= df_Info.page_size;
      page++;
   }
}

//------------------------------------------------------------------------------------
//  Записать данные
//	Параметры: count     -> Количество байт, которое необходимо записать
//  		   *buffer    -> Адрес буффера в котором назодятся исходные данные
//------------------------------------------------------------------------------------
void df_LinearSpace::UnsafeWrite( uint8 *buffer, uint16 count )
{
   uint16 page_space_left = df_Info.page_size - address;

   // загружаем данные в буффер, если они еще не там.
   if ( page != page_in_buffer1 )
   {
      df_PageFunc(DF_FLASH_TO_BUF1, page);
      page_in_buffer1 = page;
   }

   while (count)
   {
      page_space_left = df_Info.page_size - address;

      if ( count > page_space_left )   // нужно записать больше данных чем осталось места на странице памяти.
      {
         // Записываем всю странцу до конца в буффере
         df_WriteStr(1, address, page_space_left, buffer);
         count -= page_space_left;
         buffer += page_space_left;

         // перемещаем буффер во флэш
         df_PageFunc(DF_BUF1_TO_FLASH_WITH_ERASE, page);
         address = 0;
         page++;

         // загружаем следующую страницу
         df_PageFunc(DF_FLASH_TO_BUF1, page);
         page_in_buffer1 = page;

      }
      else
      {
         df_WriteStr(1, address, count, buffer);
         address += count;
         count = 0;
      }
   }
}

//------------------------------------------------------------------------------------
//  Сохранить буффер записи в физической памяти.
//------------------------------------------------------------------------------------
void df_LinearSpace::Finalize()
{
   df_PageFunc(DF_BUF1_TO_FLASH_WITH_ERASE, page);
}

//------------------------------------------------------------------------------------
//  Переместить указатель считывания вперед
//	Параметры: displacement -> Смещение
//------------------------------------------------------------------------------------
void df_LinearSpace::SeekForward( uint32 displacement )
{
   page += (displacement / df_Info.page_size);
   address += (displacement % df_Info.page_size);
}

//------------------------------------------------------------------------------------
//  Переместить указатель считывания назад
//	Параметры: displacement -> Смещение
//------------------------------------------------------------------------------------
void df_LinearSpace::SeekBackward( uint32 displacement )
{
   page -= (displacement / df_Info.page_size);
   address -= (displacement % df_Info.page_size);
}


//------------------------------------------------------------------------------------
//  Переместить указатель в заданную точку
//------------------------------------------------------------------------------------
void df_LinearSpace::GoTo( uint32 location )
{
   page = (location / df_Info.page_size);
   address = (location % df_Info.page_size);
}

#endif /* DF_ENABLE_LINEAR_SPACE */