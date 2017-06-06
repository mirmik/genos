#ifndef GENOS_CHARDEV_H
#define GENOS_CHARDEV_H

#include <kernel/file/File.h>

namespace Genos {
	class SerialDevice {
		uint16_t refs = 0;

		//Кольцевые буферы.
		gxx::ByteRing rxring;
		gxx::ByteRing txring;

		//Справочная информация, подсказывающая, когда следует будить потоки.
		size_t needToWrite;
		size_t needToRead;

		//Очереди ожидания.
		WaitQueue rxwait;
		WaitQueue txwait;

	public:
		void setRxBufferSize(size_t sz) { rxring.change_size(sz); }
		void setTxBufferSize(size_t sz) { txring.change_size(sz); }
	
		int32_t write(const char* data, size_t size, uint16_t flags) override {
			__label__ routine;
			if (size == 0) return 0;
			
			int writed = 0;

			irqstate_t save = global_irqs_save();

			if (!txwait.empty()) {
				if (flags && O_NONBLOCK) return 0;
				WaitQueueNode waitnode(current_schedee());
				txwait.wait(waitnode);
				__displace__();
			}
		
			routine:
			if (txring.empty()) {
				writed += startDataWrite(data, size);
			};
			writed += txring.write(data + writed, size - writed);
		
			if (writed != size) {
				if (flags && O_NONBLOCK) return writed;

				needToWrite = size - writed; 
				WaitQueueNode waitnode(current_schedee());
				txwait.wait_priority(waitnode);

				__displace__();
				goto routine;
			}	

			global_irqs_restore(save);
		
			canWrite();
			return ret;
		}

		int32_t read(char* data, size_t size, uint16_t flags) override {
			
		}

	protected:
		//Метод, инициализирующий запись данных. Вызывается при пустом буффере вывода.
		//устройство отберёт первую партию данных. Остальные данные должны быть помещены
		//в txring методом write.
		virtual int32_t startDataWrite(const char* data, size_t maxsz) = 0; 
		
		//Пробудить ожидающего чтения.
		void canRead();
		
		//Пробудить ожидающего записи.
		void canWrite() {
			if (!txwait.empty()) {
				txwait.unwait_one();
			}
		}	
	};
}

#endif