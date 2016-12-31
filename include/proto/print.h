#ifndef GENOS_PRINT
#define GENOS_PRINT

#include <defines/size_t.h>
#include <assert.h>
#include <string.h>
#include <utilxx/numput.h>
#include <util/ascii_convert.h>
#include <util/retype.h>

//#include <stdarg.h>
//#include <printf_impl.h>

//static void writer_printf_helper(void* d, char c);

class Print {
public:
	
	virtual int putc(char c) {
		write(&c,1);
	}

	virtual int write(const char*, size_t) = 0;

	/*inline int __putcharWrite(const char* data, size_t size) {
		const char* end = data + size;
		size_t ret = 0;

		for (data != end) {
			ret += putchar(data++);
		};

		return ret;
	}*/

	int print(const char* str) {
		return write(str, strlen(str));
	}

	int print(int8_t num, uint8_t base = 10) {
		char buf[32];
		auto ptr = backward_numput_simple(buf, 32, num, base);
		return write(ptr, buf + 32 - ptr);
	}

	int print(int16_t num, uint8_t base = 10) {
		char buf[32];
		auto ptr = backward_numput_simple(buf, 32, num, base);
		return write(ptr, buf + 32 - ptr);
	}

	int print(int32_t num, uint8_t base = 10) {
		char buf[32];
		auto ptr = backward_numput_simple(buf, 32, num, base);
		return write(ptr, buf + 32 - ptr);
	}

	int print(int64_t num, uint8_t base = 10) {
		char buf[48];
		auto ptr = backward_numput_simple(buf, 48, num, base);
		return write(ptr, buf + 48 - ptr);
	}

	int print(uint8_t num, uint8_t base = 10) {
		char buf[32];
		auto ptr = backward_numput_simple(buf, 32, num, base);
		return write(ptr, buf + 32 - ptr);
	}

	int print(uint16_t num, uint8_t base = 10) {
		char buf[32];
		auto ptr = backward_numput_simple(buf, 32, num, base);
		return write(ptr, buf + 32 - ptr);
	}

	int print(uint32_t num, uint8_t base = 10) {
		char buf[32];
		auto ptr = backward_numput_simple(buf, 32, num, base);
		return write(ptr, buf + 32 - ptr);
	}

	int print(uint64_t num, uint8_t base = 10) {
		char buf[48];
		auto ptr = backward_numput_simple(buf, 48, num, base);
		return write(ptr, buf + 48 - ptr);
	}

	int printhex(uint8_t num) {
		char buf[2];
		uint8_to_hex(buf, num);
		return write(buf,2);
	};

	int printhex(uint16_t num) {
		char buf[4];
		uint16_to_hex(buf, num);
		return write(buf,4);
	};

	int printhex(uint32_t num) {
		char buf[8];
		uint32_to_hex(buf, num);
		return write(buf,8);
	};

	int printhex(uint64_t num) {
		char buf[16];
		uint64_to_hex(buf, num);
		return write(buf,16);
	};

	int printhex(int8_t num) { return printhex((uint8_t) num); };
	int printhex(int16_t num) { return printhex((uint16_t) num); };
	int printhex(int32_t num) { return printhex((uint32_t) num); };
	int printhex(int64_t num) { return printhex((uint64_t) num); };

	int println() {
		return write("\r\n", 2);
	}

	template <typename T0>
	int println(T0 arg0) {
		int ret = print(arg0);
		ret += println();
		return ret;
	}

	template <typename T0, typename T1>
	int println(T0 arg0, T1 arg1) {
		int ret = print(arg0, arg1);
		ret += println();
		return ret;
	}

	template <typename T0>
	int printhexln(T0 arg0) {
		int ret = printhex(arg0);
		ret += println();
		return ret;
	}

	template <typename T0, typename T1>
	int printhexln(T0 arg0, T1 arg1) {
		int ret = printhex(arg0, arg1);
		ret += println();
		return ret;
	}

	/*int vprintf(const char* format, va_list args) {
		__print(writer_printf_helper, this, format, args);
	};

	int printf(const char* format, ...) {
		int ret;
		va_list args;

		assert(format != NULL);

		va_start(args, format);
		ret = vprintf(format, args);
		va_end(args);

		return ret;
	};*/
};

/*static void writer_printf_helper(void* d, char c) {
	RETYPE(genos::StreamWriter*, w, d);
	w->putc(c);
}*/

#endif