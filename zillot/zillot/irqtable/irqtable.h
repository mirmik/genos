#ifndef ZILLOT_HAL_IRQTBL_H
#define ZILLOT_IRQTBL_H

/*
	irqtable - это комутатор прерываний genos. 
	Если архитектура использует irqtable, все вызовы прерываний перенаправляются в эту подсистему.
	Целью irqtable является унификация работы с прерываниями и упрощение их отслеживания и отладки.
*/

#include <zillot/irq.h>

/// Запись таблицы хранит информацию об одном из прерываний.
struct irq_record {
	irq_handler_t handler;
	void* handler_arg;
	volatile uint16_t count;
};
	
__BEGIN_DECLS

// Возвращает 1, если управление внутри обработчика irqtable 
unsigned char is_interrupt_context();

// Инициализировать систему (необходимо выполнить до возникновения первого прерывания). 
void irqtable_init();

// Установить обработчик. Номера прерываний по соглашению храняться в periph/irqdefs.h
void irqtable_set_handler(int irqno, irq_handler_t handler, void* arg);

// Обработчик заглушка для отлова неинициализированных прерываний. 
void irqtable_stub(void* irqno);

void irqtable_debug_print();

void do_irq(uint8_t i);

__END_DECLS

#ifdef __cplusplus
#include <igris/shell/conscmd.h>
extern igris::console_command irqtable_utilities[];

namespace genos
{
	namespace irqtable 
	{
		uint16_t counter(int irqno);
	}
}
#endif

#endif