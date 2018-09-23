/*************************************************************************************
*
*  Драйвер Atmel DataFlash
*
*  by:      BSVi
*  version: 1.00
*
*************************************************************************************/

#ifndef _DATA_FLASH_H
#define _DATA_FLASH_H

// В этом файле даны краткие описания функций. Подробную информацию
// о принимаемых и возвращаемых параметрах конкретной функции можно в
// "шапке" этой функции в файле dataflash.cpp


// Закомментируйте эту строчку, чтобы отключить автоопределение типа подключенной
// микросхемы dataflash во время выполнения программы
#define DF_AUTODETECT_FEATURES

// Закомментируйте эту строчку, если класс df_LinearSpace не используется
#define DF_ENABLE_LINEAR_SPACE

// Описание возможностей подключенной микросхемы.
struct df_Info_t
{
   uint16  pages;       // Количество страниц
   uint16  page_size;   // Размер страницы в байтах
   uint8   page_bit;    // Длина адреса страницы
   uint8   chip_id;     // Код устройства
};

extern df_Info_t df_Info;

// Возвращает S_OK если микросхема подключена. После вызова этой функции,
// парамтеры подключеной микросхемы можно узнать из структуры df_Info
Result df_Init();

// Проверка выполнилась ли предыдущая операция. Сейчас драйвер сам проверяет
// выполнение операций. Нет необходимости использовать эту функцию.
uint8 df_isReady();

// Запись и чтение в буффер. См. dataflash.cpp
void df_WriteStr( uint8 BufferNo, uint16 Addr, uint16 Count, uint8 *BufferPtr );
void df_WriteByte( uint8 BufferNo, uint16 Addr, uint8 Data );

void df_GetStr( uint8 BufferNo, uint16 Addr, uint16 Count, uint8 *BufferPtr );
uint8 df_GetChar(uint8 BufferNo, uint16 Addr );

// Чтение непосредственно из флэш-памяти
void df_FlashRead( uint16 PageAdr, uint16 Addr, uint16 Count, uint8 *BufferPtr );

void df_PageFunc( uint8 PageCmd, uint16 PageAdr );
/*****************************************************************************
*  Коды комманд для использования с df_PageFunc
******************************************************************************/
#define DF_FLASH_TO_BUF1 		0x53	/* Main memory page to buffer 1 transfer  */
#define DF_FLASH_TO_BUF2 		0x55	/* Main memory page to buffer 2 transfer */
#define DF_BUF1_TO_FLASH_WITH_ERASE   	0x83	/* Buffer 1 to main memory page program with built-in erase */
#define DF_BUF2_TO_FLASH_WITH_ERASE   	0x86	/* Buffer 2 to main memory page program with built-in erase */
#define DF_BUF1_TO_FLASH_WITHOUT_ERASE  0x88	/* Buffer 1 to main memory page program without built-in erase */
#define DF_BUF2_TO_FLASH_WITHOUT_ERASE  0x89	/* Buffer 2 to main memory page program without built-in erase */


/*****************************************************************************
*  Надстройка для удобного чтения и записи информации
******************************************************************************/
#ifdef DF_ENABLE_LINEAR_SPACE
// Класс представляет dataflash как линейное пространство и избавляет разработчика от
// необходимости следить за структурой памяти, распределением страниц, их перезаписью,
// загрузкой и выгрузкой в/из буфферов. Во многих простых случаях он может заменить
// файловую систему.
//
// Клас предоставляет два метода записи - Write и UnsafeWrite. Write просто записывает данные
// и программисту больше не о чем волноваться. UnsafeWrite предназначена  для последовательной
// записи маленьких порций данных. После нескольких последовательных вызовов UnsafeWrite перед любой
// другой операцией с dataflash должен быть вызван метод Finalize, иначе содержимое буффера
// может быть переписано до того, как будет перенесено в основную флэш-память.
//
// GoToZero отличается от GoTo(0) тем, что первый рбаотает значительно быстрее.
//
// Все функции не производят проверку на пересечения границ памяти. Вы должны сами следать за этим.
// узнать количество байт от текущей точки до конца памяти можно методом SpaceToEnd
//
// Более подробные описания принимаемых параметров см. dataflash.cpp

class df_LinearSpace
{
  private:
   uint16 page;
   uint16 address;
   static uint16 page_in_buffer1;

  public:
   inline df_LinearSpace() { GoToZero(); }
   void GoToZero();
   uint32 SpaceToEnd();
   void Finalize();
   void Read( uint8 *buffer, uint16 count );
   void UnsafeWrite( uint8 *buffer, uint16 count );
   void Write( uint8 *buffer, uint16 count ) { UnsafeWrite(buffer, count); Finalize();}
   void SeekForward(  uint32 displacement );
   void SeekBackward(  uint32 displacement );
   void GoTo( uint32 location );
};

#endif /* DF_ENABLE_LINEAR_SPACE */
#endif /* _DATA_FLASH_H */
