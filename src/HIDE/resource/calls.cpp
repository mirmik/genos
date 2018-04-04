#include <resource/calls.h>
#include <sched/Schedee.h>
#include <resource/descriptor.h>
#include <fs/file.h>

namespace genos {

	int open(const char* path, uint16_t flags) {

	}

	int openptr(genos::file* filp, uint16_t flags) {
		int no = push_descriptor();
		new (&genos::current_schedee()->descriptors[no]) descriptor(descriptor::DescType::File, flags, filp);		
		return no;
	}

	int close(int no) {
		descriptor* desc = get_descriptor(no);
		if (!desc) return -1;

		close_descriptor(*desc);
		desc->type = descriptor::DescType::Nil;
		return 0;
	}

	int write(int fd, const char* data, size_t size) {
		descriptor* desc = get_descriptor(fd);
		if (desc->type != genos::descriptor::DescType::File) return -1;
		genos::file* flp= reinterpret_cast<genos::file*>(desc->body);
		
		//TODO
		flp->write(data, size, desc->fpos);
		return 0;
	}

	int read(int fd, char* data, size_t size) {

	}

	int pipe() {

	}

}