#ifndef IGRIS_CTROBJ_H
#define IGRIS_CTROBJ_H

// Объект синхронизации для построения очередей, корутин и прочей гадости.
// Живёт в памяти процесса или его структуре и подключается
// в очереди ожидания (см. файл wait.h)

#include <igris/container/dlist.h>
#include <stdint.h>

#define CTROBJ_SCHEDEE_LIST 0
#define CTROBJ_WAITER_SCHEDEE 1
#define CTROBJ_WAITER_DELEGATE 2
#define CTROBJ_KTIMER_SCHEDEE 3
#define CTROBJ_KTIMER_DELEGATE 4

struct ctrobj
{
    igris::dlist_node lnk = {};
    intptr_t future = 0;
    uint8_t type = 0;

public:
    ctrobj(uint8_t type)
    {
        this->type = type;
    }

    void init(uint8_t type)
    {
        this->type = type;
        lnk.unlink();
    }
};

void ctrobj_deinit(struct ctrobj *obj);
intptr_t ctrobj_get_future(struct ctrobj *obj);
void ctrobj_set_future(struct ctrobj *obj, intptr_t future);

#endif
