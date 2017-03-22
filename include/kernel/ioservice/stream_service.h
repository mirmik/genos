#ifndef GENOS_STREAM_SERVICE_H
#define GENOS_STREAM_SERVICE_H

struct stream_service;

struct stream_service_operations {
	int(*write)(struct stream_service*, const char* data, size_t sz);
	int(*read)(struct stream_service*, const char* data, size_t sz);
	int(*avail)(struct stream_service*, const char* data, size_t sz);
	int(*flush)(struct stream_service*, const char* data, size_t sz);
};

struct stream_service {
	struct service;

	stream_service_operations strmops;
	void * privdata;	
};

__BEGIN_DECLS

static void stream_service_routine(struct stream_service * strmsrvs) {

}

/*static void construct_stream_service() {
	construct_autom_schedee(void(*routine)(void*), void* localstorage) 
}*/

__END_DECLS


#endif