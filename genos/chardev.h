#ifndef GENOS_CHARDEV_H
#define GENOS_CHARDEV_H

#include <igris/datastruct/dlist.h>
#include <igris/container/static_string.h>
#include <genos/resource/file_head.h>
#include <zillot/common/chardev.h>
#include <igris/osinter/wait.h>

namespace genos 
{
	class chardev : public genos::file_head
	{
		igris::static_string<8> _name;

	public:
		chardev(const char * name) : _name(name) {}
	};

	class zillot_chardev : public genos::chardev 
	{
		zillot::chardev * zchar = nullptr;
		dlist_head rx_wait = DLIST_HEAD_INIT(rx_wait);
		dlist_head tx_wait = DLIST_HEAD_INIT(tx_wait);

	public:
		zillot_chardev(zillot::chardev * zchar, const char * name);		
		int write(const void * data, unsigned int size) override;
		int read(void * data, unsigned int size) override;
		int on_open() override;
		int on_release() override;
		void rx_callback();
		void tx_callback();
	};
}

#endif