#include <kernel/service/table.h>
#include <datastruct/hashtable.h>

#include <utilxx/goptions.h>
#include <stdbool.h>
#include <assert.h>

static_assert(OPTION(hashtable_size) > 0, "Wrong service hashtable size");
#define SERVICE_HASHTABLE_SIZE OPTION(hashtable_size)

static qid_t __cur_qid;
static qid_t __max_qid = 1000;

static struct hlist_head __servs_ht_arr[SERVICE_HASHTABLE_SIZE]; 
static struct hashtable __service_hashtable; 

int sh_equal (void* lkey, void* rkey) { return *(int32_t*)lkey == *(int32_t*)rkey; }
size_t sh_hash (void* key) { return *(int32_t*)key ^ 0xABCDEFAB; }
void* sh_getkey (struct hlist_node* lnk) { return &container_of(lnk, struct service, hlnk)->qid; }


qid_t get_new_qid() {
	struct service* __;
	++__cur_qid;
	while (hashtable_get(&__service_hashtable, &__cur_qid))
		if (__cur_qid > __max_qid) __cur_qid = 0; 
	return __cur_qid;
}

void service_table_init() {
	hashtable_init(&__service_hashtable, __servs_ht_arr, SERVICE_HASHTABLE_SIZE,
		sh_getkey, sh_hash,sh_equal);
}

void register_stable_service(struct service* s, qid_t qid) {
	s->qid = qid;
	hashtable_put(&__service_hashtable, &s->hlnk);
}

qid_t register_service(struct service* s) {
	s->qid = get_new_qid();
	hashtable_put(&__service_hashtable, &s->hlnk);
	return s->qid;
}

struct service* get_service(qid_t qid) {
	return hashtable_get_entry(&__service_hashtable, &qid, struct service, hlnk);
}

