#ifndef DEVICES_NAMESPACE_H
#define DEVICES_NAMESPACE_H

#include <genos/drivers/device_head.h>
#include <genos/resource/namespace.h>

struct devices_namespace
{
    struct namespace_head ns;
};

__BEGIN_DECLS

void devices_namespace_init(struct devices_namespace *devns, const char *name);

__END_DECLS

#endif