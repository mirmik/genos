#ifndef GENOS_HAL_IRQTBL_H
#define GENOS_HAL_IRQTBL_H

/*
	irqtable - это комутатор прерываний genos. 
	Если архитектура использует irqtable, все вызовы прерываний перенаправляются в эту подсистему.
	Целью irqtable является унификация работы с прерываниями и упрощение их отслеживания и отладки.
*/

#include <hal/irq.h>

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

__END_DECLS

#ifdef __cplusplus
namespace genos
{
	namespace irqtable 
	{
		uint16_t counter(int irqno);
	}
}
#endif

/*#ifdef __cplusplus
#include <igris/event/delegate.h>

namespace genos 
{
	namespace irqtable 
	{
		static inline void set_handler(int irqno, irq_handler_t handler, void* arg) 
		{
			irqtable_set_handler(irqno, handler, arg);
		}

		template<class T>
		void set_handler(int irqno, void(T::* handler)(), T* arg) 
		{
			auto dlg = igris::make_fastdelegate(handler, arg); 
			set_handler(irqno, dlg.extfunction, dlg.object);
		}
	}
}
#endif
*/
#endif