#ifndef GENOS_CDEV_H
#define GENOS_CDEV_H

#include <genos/fs/file.h>
#include <gxx/event/flag.h>

namespace genos {
	struct cdev : public genos::file {};
	struct cstream : public genos::cdev {
		bool is_cstream() override { return true; }
		gxx::event::action_flag avail_flag;
		gxx::event::action_flag empty_flag;
		virtual int avail() = 0;
		virtual int room() = 0;
	};
}

#endif