#ifndef KERNEL_PIPE_INTERFACE
#define KERNEL_PIPE_INTERFACE

struct stream;
struct stream_ops;

typedef struct stream stream;
typedef struct stream_ops stream_ops;

typedef int(*func_write_t)(struct stream *obj, const char *data, size_t size);
typedef int(*func_read_t)(struct stream *obj, const char *data, size_t size);
typedef int(*func_flush_t)(struct stream *obj);

typedef int(*func_unlink_t)(struct stream *obj);

struct stream_ops {
	func_write_t write;
	func_read_t read;
	func_flush_t flush;

	func_unlink_t unlink;
};

struct stream {
	struct stream_ops ops;
	void* private;
};

#define stream_write(strm,data,size) (strm)->write((data),(size))
#define stream_read(strm,data,size) (strm)->read((data),(size))
#define stream_flush(strm,data,size) (strm)->flush()

#define stream_unlink(strm,data,size) (strm)->unlink()

#endif