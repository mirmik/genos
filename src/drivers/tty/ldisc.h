#ifndef GENOS_CDEV_LDISC_H
#define GENOS_CDEV_LDISC_H

namespace genos 
{
	class tty;

	class ldisc 
	{
	public:
		genos::tty * tty;

	public:
		virtual void receive_byte(char c) = 0;
		virtual void write(const char* data, size_t size);
	};
}

#endif