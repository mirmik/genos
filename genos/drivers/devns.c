#include <genos/drivers/devns.h>
#include <genos/drivers/device_head.h>
#include <igris/util/pathops.h>

int devices_namespace_lookup(
    struct namespace_head * ns,
    struct file_head ** filpp,
    const char * name)
{
	(void) ns;
	struct device_head * head;
	dlist_for_each_entry(head, &devices_list, lnk_devices_list)
	{
		if (strncmp(name, head->name, DEVICE_NAME_MAXLEN) == 0)
		{
			*filpp = &head->fil;
			return 0;
		}
	}

	*filpp = 0;
	return -1;
}

const struct namespace_operations devices_namespace_ops =
{
	.lookup = devices_namespace_lookup
};

void devices_namespace_init(struct devices_namespace * devns, const char * name)
{
	namespace_init(&devns->ns, name, &devices_namespace_ops);
}