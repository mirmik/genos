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
		zillot_chardev(zillot::chardev * zchar, const char * name) : 
			genos::chardev(name), zchar(zchar) 
		{
			zchar->rx_callback = igris::make_delegate(&zillot_chardev::rx_callback, this);
			zchar->tx_callback = igris::make_delegate(&zillot_chardev::tx_callback, this);
		}
		
		int write(const void * data, unsigned int size) override 
		{
			return zchar->write(data, size);
		}

		int read(void * data, unsigned int size) override 
		{
			if (zchar->avail() == 0) 
			{
				wait_current_schedee(&rx_wait, 0, nullptr);
			}

			if (data == nullptr)
				return 0;

			return zchar->read(data, size);
		}
		
		int on_open() override 
		{
			return 0;
		}
		
		int on_release() override 
		{
			return 0;
		}

		void rx_callback() 
		{
			unwait_one(&rx_wait, 0);
		}

		void tx_callback() 
		{
			unwait_one(&tx_wait, 0);
		}
	};
}

#endif