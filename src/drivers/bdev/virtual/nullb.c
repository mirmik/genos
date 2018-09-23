#include <drivers/bdev/virtual/nullb.h>

static int nullb_write_block (struct block_device * dev, char * buf, size_t sz, int blkno) {
	(void) dev; (void) buf; (void) blkno; 
	return sz;
}

static int nullb_read_block (struct block_device * dev, char * buf, size_t sz, int blkno) {
	(void) dev; (void) buf; (void) blkno; 
	return 0;
}

struct block_device_operations nullb_ops = {
	.write_block = nullb_write_block,
	.read_block = nullb_read_block,
};

void link_nullb_device(const char * dir) {

}