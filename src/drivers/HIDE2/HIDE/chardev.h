#ifndef GENOS_CHARDEV_H
#define GENOS_CHARDEV_H



class UartDeviceDriver {
public:
	virtual int putc(char c);
	virtual int getc();

	virtual bool avail();
	virtual bool ready();
	
	virtual void setRXHandler(Kernel::IRQHandler handler);
	virtual void setTXHandler(Kernel::IRQHandler handler);

	virtual void enableRXInt(bool b);
	virtual void enableTXInt(bool b);

	virtual void enableRX(bool b);
	virtual void enableTX(bool b);
};

#endif