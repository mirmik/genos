#include <gxx/buffer.h>
#include <string.h>

void gxx::buffer_fill(const gxx::buffer& buffer, char c) {
	memset(buffer.data(), c, buffer.size());
};