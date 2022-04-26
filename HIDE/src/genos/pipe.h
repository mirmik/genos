#ifndef GENOS_PIPE_H
#define GENOS_PIPE_H

#include <genos/resource.h>

namespace genos 
{
	class pipe : public node
	{
	public:
		int write(const void* data, size_t size, int flags) override;
		int read(void* data, size_t size, int flags) override;		
		int room() override;		
		int avail() override;
	}

	class nobuf_pipe : public pipe
	{
		dlist_head wr_wait = DLIST_HEAD_INIT(&wr_wait);
		uint8_t refs = 0;
		const char * msg;
		size_t msglen;

	public:
		int write(const void* data, size_t size, int flags) override 
		{
			if (refs < 2) return -1;

			while (msg != nullptr) 
			{
				wait_current_schedee(&wr_wait);
			}

			msg = data;
			msglen = size;
			
			if (dlist_empty(&wr_wait))
				unwait_one(&wr_wait);

			return 0;
		}

		int read(void* data, size_t size, int flags) override 
		{
			if (refs < 2) return -1;

			while (msg == nullptr) 
			{
				wait_current_schedee(&wr_wait);
			}
			
			memcpy(data, msg, min(size, msglen));
			msglen = nullptr;

			if (dlist_empty(&wr_wait))
				unwait_one(&wr_wait);

			return 0;
		}	

		int avail() override { return msg ? msglen : 0; }

		void clean() 
		{
			wr_wait = DLIST_HEAD_INIT(&wr_wait);
			refs = 0;
			msg = nullptr;
			msglen = 0;
		}
	}
}

#endif