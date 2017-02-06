#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <proto/print.h>
#include <kernel/wait/wait.h>

/*FileAbstract это еще не файл.
FileAbstract ничего не зает об ОС и не умеет работать 
с блокирующим вводом, реализуя только асинхронный интерфейс
Однако, он предоставляет функцию wait для надстройки 
блокирующего io*/

class FileAbstract : public Print {
public:
	virtual char getc() {
		char c;
		read(&c, 1);
		return c;
	}

	virtual int write(const char* data, size_t size) = 0;
	virtual int read(char* data, size_t size) = 0;
	
	virtual int avail() = 0;
	virtual int room() = 0;

	virtual int flush() = 0;

	/*Файлы устройств могут поддерживать waiter-ы для
	сигнализации другим частям системы о том, что */
	virtual bool waitread(WaiterBasic& waiter) { return false; };
	virtual bool waitwrite(WaiterBasic& waiter) { return false; };
};

class DeviceFile : public FileAbstract {};

#endif