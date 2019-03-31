#ifndef GENOS_DRIVERS_I2C_H
#define GENOS_DRIVERS_I2C_H

#include <assert.h>

#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>
#include <sched/api.h>
#include <sched/wait.h>

struct i2c_device
{
	uint8_t _lock;
	DLIST_HEAD(wlnk);

	virtual void write_start(uint8_t target, const void* data, uint16_t size);
	virtual void writeread_start(uint8_t target, const void* out, uint16_t olen, void* in, uint16_t ilen);
	virtual int status();
	
	int write(uint8_t target, const void* data, uint16_t size)
	{
		int ret;
		write_start(target, data, size);

		ret = wait_current_schedee(&wlnk, 1);
		if (ret == -1) 
		{
			volatile uint8_t flag = 0;
			wait_flag_setted(&wlnk, 1);
		}
		
		return status();
	}

	int writeread(uint8_t target, const void* out, uint16_t olen, void* in, uint16_t ilen)
	{
		int ret;
		writeread_start(target, out, olen, in, ilen);
		
		ret = wait_current_schedee(&wlnk, 1);
		if (ret == -1) 
		{
			volatile uint8_t flag = 0;
			wait_flag_setted(&wlnk, 1);
		}

		return status();
	}

	int lock()
	{
		while (1)
		{
			system_lock();

			if (0 != _lock)
			{
				system_unlock();
				wait_current_schedee(&wlnk, 0);
			}
			else break;
		}

		system_unlock();
	}

	int unlock()
	{
		unwait_one(&wlnk);
	}
};

#endif