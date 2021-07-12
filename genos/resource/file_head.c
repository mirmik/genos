#include <genos/resource/file_head.h>


void file_head_init(struct file_head * file, const struct file_operations * ops) 
{
	file->f_ops = ops;
	file->refs = 0;
}