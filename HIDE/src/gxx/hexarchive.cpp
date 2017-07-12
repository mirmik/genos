#include <gxx/future/hexarchive.h>
#include <util/ascii_convert.h>
#include <gxx/debug/dprint.h>

namespace gxx {

	bool HexArchive::check_valid() {
		if (m_size % 2 == 1) {__valid = false; return false;};
		return true;
	}

	bool HexArchive::is_valid() {
		return __valid;
	}


	bool HexArchive::can(uint8_t len) {
		if (__valid == false) return false;
		if (m_cursor - m_data + len > m_size) { __valid = false; return false; }
		else return true; 
	}

	uint8_t HexArchive::canread() {
		return m_cursor - m_data - m_size;
	}

	int HexArchive::__pull(void* ptr, int len) {
		if (!can(len)) return -1;
		bhex2bytes(ptr, m_cursor, len);
		m_cursor += len * 2;
		return len;
	}

	int HexArchive::pull(uint8_t& ref) { return __pull(&ref, 1); }; 
	int HexArchive::pull(uint16_t& ref) { return __pull(&ref, 2); }; 
	int HexArchive::pull(uint32_t& ref) { return __pull(&ref, 4); }; 
	int HexArchive::pull(uint64_t& ref) { return __pull(&ref, 8); }; 
	int HexArchive::pull(int8_t& ref) { return __pull(&ref, 1); }; 
	int HexArchive::pull(int16_t& ref) { return __pull(&ref, 2); }; 
	int HexArchive::pull(int32_t& ref) { return __pull(&ref, 4); }; 
	int HexArchive::pull(int64_t& ref) { return __pull(&ref, 8); }; 

	int HexArchive::pull(char*& ref, size_t sz) {
		if (!can(sz)) return -1;
		memcpy(ref, m_cursor, sz);
		m_cursor += sz;	
		return sz;
	};

	int HexArchive::__push(void* ptr, int len) {
		if (!can(len)) return -1;
		bytes2bhex(m_cursor, ptr, len);
		m_cursor += len * 2;
		return len;
	}; 

	int HexArchive::push(uint8_t& ref) { return __push(&ref, 1); }; 
	int HexArchive::push(uint16_t& ref) { return __push(&ref, 2); }; 
	int HexArchive::push(uint32_t& ref) { return __push(&ref, 4); }; 
	int HexArchive::push(uint64_t& ref) { return __push(&ref, 8); }; 
	int HexArchive::push(int8_t& ref) { return __push(&ref, 1); }; 
	int HexArchive::push(int16_t& ref) { return __push(&ref, 2); }; 
	int HexArchive::push(int32_t& ref) { return __push(&ref, 4); }; 
	int HexArchive::push(int64_t& ref) { return __push(&ref, 8); }; 

	int HexArchive::push(char*& ref, size_t sz) {
		if (!can(sz)) return -1;
		memcpy(m_cursor, ref, sz);
		m_cursor += sz;	
		return sz;
	};
};