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
    return -1;
}

genos::autom_schedee::autom_schedee(autom_schedee_func_t foo, void *privdata)
{
    this->state = 0;
    this->func = foo;
    this->priv = privdata;
}

void genos::autom_schedee::init(autom_schedee_func_t foo, void *privdata)
{
    this->state = 0;
    this->func = foo;
    this->priv = privdata;
}