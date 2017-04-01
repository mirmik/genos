#ifndef GXX_POOL_H
#define GXX_POOL_H

#include <mem/sysalloc.h>

#include <gxx/dlist.h>
#include <datastruct/slist_head.h>
#include <gxx/vector.h>

namespace gxx {

	class page_pool {
	private:
		//gxx::vector<void*> pages;
		
	public:
		void* allocate_page(size_t pageSize) {
			void* ret = sysalloc(pageSize);
			//pages.emplace_back(ret);
			return ret;
		}	

		~page_pool() {
			//for(auto p : pages) {
			//	sysfree(p);
			//}
		}	
	};

	class block_pool : public page_pool {
	private:
		slist_head head;
		size_t objsize;
		size_t totalOnPage;

	public: 
		block_pool(size_t objsize, size_t total) : 
			objsize(objsize), totalOnPage(total) {
				slist_init(&head);
			};

		void* allocate() {
			if (head.next == nullptr) formatNewPage();
			void* ret = static_cast<void*>(head.next);
			slist_del(head.next, &head);
			return ret;
		}

		void release(void* ptr) {

		}

	private:
		void formatNewPage() {
			assert(head.next == nullptr);
			size_t pageSize = objsize * totalOnPage;

			void* page = allocate_page(pageSize);
			
			char * end = (char*)page + pageSize;
			for(char* pos = (char*)page; pos != end; pos += objsize) {
				slist_add_next(reinterpret_cast<slist_head*>(pos), &head);
			}
		}
	};
}

#endif