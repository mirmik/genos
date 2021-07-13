#include <genos/drivers/stream_adapter.h>
#include <igris/sync/syslock.h>
#include <genos/wait.h>
#include <igris/osinter/wait.h>

void stream_adapter_tasklet_routine(void * tasklet_priv)
{
	int need = 0;

	struct stream_adapter * adapter = (struct stream_adapter *) tasklet_priv;
	system_lock();

	if (!dlist_empty(&adapter->rqueue))
	{
		if (adapter->stream_ops->avail(adapter->priv))
		{
			unwait_one(&adapter->rqueue, NULL);
		}
		need = 1;
	}

	if (!dlist_empty(&adapter->wqueue))
	{
		if (adapter->stream_ops->room(adapter->priv))
		{
			unwait_one(&adapter->wqueue, NULL);
		}
		need = 1;
	}

	if (!need)
		tasklet_unplan(&adapter->checker);

	system_unlock();
}

int stream_adapter_write(
	struct file_head * head,
	const void * data, unsigned int size)
{
	struct stream_adapter * adapter = mcast_out(head, struct stream_adapter, dev.fil);

	char *       cdata = (char *) data;
	unsigned int csize = size;
	int writed;

	while (adapter->stream_ops->room(adapter->priv) == 0)
	{
		if (adapter->dev.fil.flags & O_NONBLOCK)
			return 0;

		wait_current_schedee(&adapter->wqueue, 0, NULL);
	}

	writed = adapter->stream_ops->write(adapter->priv, cdata, csize);
	return writed;
}

int stream_adapter_read(void * data, unsigned int size)
{
	char *       cdata = (char *) data;
	unsigned int csize = size;
	int readed;

	while (adapter->stream_ops->avail(adapter->priv) == 0)
	{
		if (adapter->dev.fil.flags & O_NONBLOCK)
			return 0;

		wait_current_schedee(&adapter->rqueue, 0, NULL);
	}

	readed = adapter->stream_ops->read(adapter->priv, cdata, csize);
	return readed;
}

const struct stream_adapter_file_ops =
{
	.write = stream_adapter_write,
	.read = stream_adapter_read,
	.on_open = NULL,
	.on_release = NULL
}

void stream_adapter_init(
    struct stream_adapter * adapter,
    const char * name,
    const struct stream_adapter_operations * ops
)
{
	adapter->stream_ops = ops;

	dlist_init(&adapter->wqueue);
	dlist_init(&adapter->rqueue);
	tasklet_init(
	    &adapter->checker,
	    stream_adapter_tasklet_routine,
	    (void*) adapter);

	device_head_init(&adapter->dev, name, stream_adapter_file_ops);
}