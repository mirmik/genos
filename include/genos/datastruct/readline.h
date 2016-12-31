#ifndef GENOS_READLINE_H
#define GENOS_READLINE_H

#include "compiler.h"
#include "assert.h"

struct readline;
static inline void readline_init(struct readline* rl, void* data, int len);
static inline int readline_putc(struct readline* rl, char c);
static inline int readline_write(struct readline* rl, const char* str, int len);
static inline char* readline_getline(struct readline* rl);
static inline void readline_reset(struct readline* rl);

struct readline {
	char* data;
	int cursor;
	int capacity;

__if_cplusplus(
	inline readline() : data(nullptr), capacity(0) {};	
	inline readline(void* _data, int _len) : data((char*)_data), capacity(_len - 1), cursor(0) {};
	
	inline void init(void* data, int len) {
		readline_init(this, data, len);
	};

	inline int write(const char* str, int len) {
		return readline_write(this, str, len);
	};

	inline int putc(const char c) {
		return readline_putc(this, c);
	};

	inline void reset() {
		readline_reset(this);
	};	

	inline char* getline() {
		return readline_getline(this);
	};
)
};

static inline void readline_init(struct readline* rl, void* data, int len) {
	assert(rl);
	rl->data = (char*) data;
	rl->capacity = len - 1;
	rl->cursor = 0;
};

static inline void readline_reset(struct readline* rl) {
	assert(rl);
	rl->cursor = 0;
};

static inline int readline_putc(struct readline* rl, char c) {
	assert(rl);
	if (rl->capacity == rl->cursor) return 0;
	*(rl->data + rl->cursor++) = c;
};

static inline int readline_write(struct readline* rl, const char* str, int len) {
	assert(rl);
	for(int i = 0; i < len; ++i)
	{
		if (rl->capacity == rl->cursor) return i;
		*(rl->data + rl->cursor++) = *(str + i);
	};
};

static inline char* readline_getline(struct readline* rl) {
	assert(rl);
	*(rl->data + rl->cursor) = 0;
	return rl->data;
};

#endif