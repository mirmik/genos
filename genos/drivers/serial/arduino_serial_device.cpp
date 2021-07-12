#include <genos/drivers/serial/arduino_serial_device.h>

DLIST_HEAD(arduino_serial_readers_waits);

int arduino_serial_device_write(struct file_head * file, const void * data, int size) 
{
	struct arduino_serial_device * dev = mcast_out(file, file);
	return dev->serial->write(data, size);
}

int arduino_serial_device_read(struct file_head * file, const void * data, int size) 
{
	struct arduino_serial_device * dev = mcast_out(file, file);
	
	if (dev->serial->available() == 0) 
	{
		dlist_add(&dev->lnk_reader, &arduino_serial_readers_waits);
		current_schedee_set_waitstate();
		return 0;
	}

	dev->serial->read(data, size);
}





void arduino_serial_device_init(
	struct arduino_serial_device * dev, 
	HardwareSerial * serial
) 
{
	dev -> serial = serial;
}


void arduino_serial_devices_step() 
{
	struct arduino_serial_device * dev;

	dlist_for_each_entry(dev, &arduino_serial_readers_waits, lnk_reader) 
	{
		if (dev->serial->available()) 
		{
			
		}
	}
}