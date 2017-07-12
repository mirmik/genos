#ifndef KERNEL_IOMANAGER_H
#define KERNEL_IOMANAGER_H

#include <genos/compiler.h>
#include <kernel/query.h>

struct iomgr_data_s {
	dlist_head reader_list;
	dlist_head writer_list;
};

__BEGIN_DECLS

//void iomgr_new_output_data(struct file* filp, size_t n);

__END_DECLS

#endif