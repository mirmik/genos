/*#include <genos/drivers/serial/arduino_serial_device.h>
#include <genos/wait.h>

DLIST_HEAD(arduino_serial_readers_waits);

int arduino_serial_device_write(struct file_head * file, const void * data, int size) 
{
	struct arduino_serial_device * dev = mcast_out(file, struct arduino_serial_device, dev.fil);
	return dev->serial->write((const char*)data, (size_t)size);
}

int arduino_serial_device_read(struct file_head * file, const void * data, int size) 
{
	struct arduino_serial_device * dev = mcast_out(file, struct arduino_serial_device, dev.fil);
	
	if (dev->serial->available() == 0) 
	{
		dlist_add(&dev->lnk_reader, &arduino_serial_readers_waits);
		wait_current_schedee(&dev->waitqueue);
		//current_schedee_set_waitstate();
		return 0;
	}

	dev->serial->read((char*)data, (size_t)size);
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
}*/