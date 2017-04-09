#ifndef GXX_POOL_H
#define GXX_POOL_H

#include <mem/sysalloc.h>

#include <gxx/dlist.h>
#include <datastruct/slist_head.h>
#include <gxx/vector.h>

namespace gxx {

	template <size_t PageSize>
	class page_pool {
		//gxx::vector<void*> pages;
		
	public:
		constexpr static size_t pageSize = PageSize; 

		void* allocate_page() {
			dprln("page_pool::allocate_page");
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

	template<typename T, size_t TotalOnPage>
	class block_pool : public page_pool<sizeof(T) * TotalOnPage> {
	private:
		using Parent = page_pool<sizeof(T) * TotalOnPage>;
		slist_head head;

	public: 
		block_pool() {
			slist_init(&head);
		};

		T* allocate() {
			if (head.next == nullptr) formatNewPage();
			T* ret = reinterpret_cast<T*>(head.next);
			slist_del(head.next, &head);
			return ret;
		}

		template<typename ... Args>
		T* emplace(Args ... args) {
			T* ptr = allocate();
			gxx::constructor(ptr, gxx::forward<Args>(args) ...);
			return ptr;
		}

		void release(T* ptr) {
			dprln("pool release TODO");
		}

	private:
		void formatNewPage() {
			assert(head.next == nullptr);
			//size_t pageSize = sizeof(T) * TotalOnPage;

			void* page = Parent::allocate_page();
			
			char * end = (char*)page + Parent::pageSize;
			for(char* pos = (char*)page; pos != end; pos += sizeof(T)) {
				slist_add_next(reinterpret_cast<slist_head*>(pos), &head);
			}
		}
	};
}

#endif