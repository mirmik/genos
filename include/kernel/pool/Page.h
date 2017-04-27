#ifndef GENOS_PAGE_H
#define GENOS_PAGE_H

#include <datastruct/dlist_head.h>
#include <gxx/dlist.h>

namespace Genos {
	class Page {
	public:
		dlist_head lnk;

		size_t pageSize;
		void* ptr;

		Page(size_t pageSize) : pageSize(pageSize) {
			dlist_init(&lnk);
		}

		void init() {
			ptr = malloc(pageSize);
		}
	};

	using PageList = gxx::dlist<Page, &Page::lnk>;
}

#endif