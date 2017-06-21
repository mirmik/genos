#include <kernel/pool/Pool.h>

Genos::PoolList Genos::pool_list;

void Genos::registry(Pool& pool, const char* name) {
	pool.name(name);
	Genos::pool_list.move_back(pool);
}

int Genos::dbginfo_pool_list() {
	for(auto& pool : pool_list) {
		dpr("pages:");
		dpr(pool.pages.size());
		dpr(' ');
		dpr("total:");
		dpr(pool.counter);
		dpr(' ');
		dpr("elsz:");
		dpr(pool.elsz);
		dpr(' ');
		dpr("onPage:");
		dpr(pool.totalOnPage);
		dpr(' ');
		dprln(pool.name());
	}
	return 0;
}