#include <igris/osinter/ctrobj.h>

void ctrobj_set_future(struct ctrobj *obj, intptr_t future)
{
    obj->future = future;
}

void ctrobj_deinit(struct ctrobj *obj)
{
    obj->lnk.unlink();
}

intptr_t ctrobj_get_future(struct ctrobj *obj)
{
    return obj->future;
}