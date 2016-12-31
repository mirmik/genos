#ifndef GXX_HEX_ARCHIVE_H
#define GXX_HEX_ARCHIVE_H

#include <gxx/future/archive.h>

namespace gxx {

	class HexArchive {
	public:
		char* m_data;
		char* m_cursor;
		size_t m_size;

		bool __valid = true;

		bool check_valid();
		bool is_valid();

		int __pull(void* ref, int len);

		int pull(uint8_t& ref);
		int pull(uint16_t& ref);
		int pull(uint32_t& ref);
		int pull(uint64_t& ref);
		int pull(int8_t& ref);
		int pull(int16_t& ref);
		int pull(int32_t& ref);
		int pull(int64_t& ref);
		int pull(char*& ref, size_t sz);

		int __push(void* ref, int len);

		int push(uint8_t& ref);
		int push(uint16_t& ref);
		int push(uint32_t& ref);
		int push(uint64_t& ref);
		int push(int8_t& ref);
		int push(int16_t& ref);
		int push(int32_t& ref);
		int push(int64_t& ref);
		int push(char*& ref, size_t sz);


		bool can(uint8_t len);

		uint8_t canread();
		int size() {
			return m_cursor - m_data;
		};

	public:
		HexArchive(const char* data, size_t size) : m_data((char*)data), m_cursor((char*)data), m_size(size) {}
	};
}

#endif 