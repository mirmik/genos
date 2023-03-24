#include <genos/autom_schedee.h>
#include <genos/displace.h>
#include <igris/util/bug.h>

void genos::autom_schedee::execute()
{
    func(priv, &state);
}

void genos::autom_schedee::finalize() {}

int genos::autom_schedee::displace()
{
    BUG();
    return -1;
}

// genos::autom_schedee::autom_schedee(void (*destructor)(genos::schedee
// *sched))
//     : schedee(destructor)
//{
// }

genos::autom_schedee::autom_schedee(autom_schedee_func_t foo,
                                    void *privdata,
                                    void (*destructor)(genos::schedee *sched))
    : schedee(destructor)
{
    this->state = 0;
    this->func = foo;
    this->priv = privdata;
}

genos::autom_schedee *genos::create_autom_schedee(autom_schedee_func_t foo,
                                                  void *privdata)
{
    genos::autom_schedee *sched = new genos::autom_schedee(
        foo, privdata, +[](genos::schedee *sched) { delete sched; });
    return sched;
}