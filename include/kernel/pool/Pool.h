#ifndef GENOS_POOL_H
#define GENOS_POOL_H

#include <gxx/dlist.h>
#include <datastruct/slist_head.h>
#include <kernel/pool/Page.h>
#include <kernel/ns/NamedObject.h>

namespace Genos {
	
	class Pool : public NamedObject {
	public:
		size_t elsz;
		size_t totalOnPage;
		PageList pages;
		size_t counter = 0;
	
		slist_head head;

		dlist_head poolslnk;
		Pool(size_t elsz, size_t totalOnPage) :
			elsz(elsz), totalOnPage(totalOnPage)
		{
			if (this->elsz < sizeof(slist_head))
				this->elsz = sizeof(slist_head); 
			dlist_init(&poolslnk); 
			slist_init(&head);
		}

	private:
		Page* allocatePage() {
			Page* page = new Page(totalOnPage * elsz);
			page->init();
			pages.move_back(*page);
			return page;
		}
	
		void formatNewPage() {
			assert(head.next == nullptr);
			Page* page = allocatePage();
			void* ptr = page->ptr;
			
			char * end = (char*)ptr + page->pageSize;
			for(char* pos = (char*)ptr; pos != end; pos += elsz) {
				slist_add_next(reinterpret_cast<slist_head*>(pos), &head);
			}
		}

	public:
		void* get() {
			if (head.next == nullptr) formatNewPage();
			void* ret = reinterpret_cast<void*>(head.next);
			slist_del(head.next, &head);
			counter++;
			return ret;
		}

		void put(void* ptr) {
			slist_head* node = reinterpret_cast<slist_head*>(ptr);
			slist_add_next(node, &head);
			counter--;
		}

		template<typename T, typename ... Args>
		T* emplace(Args ... args) {
			assert(sizeof(T) == elsz);
			T* ptr = (T*) get();
			gxx::constructor(ptr, gxx::forward<Args>(args) ...);
			return ptr;
		}

		template<typename T>
		void release(T* ptr) {
			gxx::destructor(ptr);
			put(ptr);
		}

		void nowFree() {
			slist_head* pos = &head;
			while(pos) {
				dprptrln(pos->next);
				if (pos->next) pos = pos->next;
				else return;
			}
		}
	};

	
	using PoolList 
		= gxx::dlist<Pool, &Pool::poolslnk>;

	void registry(Pool& pool, const char* name);
	int dbginfo_pool_list();

	extern PoolList pool_list;
}

#endif