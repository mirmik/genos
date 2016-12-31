#ifndef GXX_ARCHIVE_H
#define GXX_ARCHIVE_H

#include <stdlib.h>
#include <stdint.h>

#include <gxx/Tuple.h>
#include <utility>

namespace gxx {

	template<typename T, typename Archive> void load(T& ref, Archive& ar)
	{
		ref.load(ar);
	};

	template<typename T, typename Archive> void save(T& ref, Archive& ar)
	{
		ref.save(ar);
	};
 
	template<typename Archive> void load(uint8_t& ref, Archive& ar) { ar.pull(ref); }; 	
	template<typename Archive> void load(uint16_t& ref, Archive& ar) { ar.pull(ref); };	
	template<typename Archive> void load(uint32_t& ref, Archive& ar) { ar.pull(ref); };	
	template<typename Archive> void load(uint64_t& ref, Archive& ar) { ar.pull(ref); };	
	template<typename Archive> void load(int8_t& ref, Archive& ar) { ar.pull(ref); };	
	template<typename Archive> void load(int16_t& ref, Archive& ar) { ar.pull(ref); };	
	template<typename Archive> void load(int32_t& ref, Archive& ar) { ar.pull(ref); };
	template<typename Archive> void load(int64_t& ref, Archive& ar) { ar.pull(ref); };
	template<typename Archive> void load(char*& ref, size_t size, Archive& ar) { ar.pull(ref,size); };

	template<typename Archive> void save(uint8_t& ref, Archive& ar) { ar.push(ref); };
	template<typename Archive> void save(uint16_t& ref, Archive& ar) { ar.push(ref); };	
	template<typename Archive> void save(uint32_t& ref, Archive& ar) { ar.push(ref); };	
	template<typename Archive> void save(uint64_t& ref, Archive& ar) { ar.push(ref); };	
	template<typename Archive> void save(int8_t& ref, Archive& ar) { ar.push(ref); };	
	template<typename Archive> void save(int16_t& ref, Archive& ar) { ar.push(ref); };	
	template<typename Archive> void save(int32_t& ref, Archive& ar) { ar.push(ref); };	
	template<typename Archive> void save(int64_t& ref, Archive& ar) { ar.push(ref); };
	template<typename Archive> void save(char*& ref, size_t size, Archive& ar) { ar.push(ref,size); };

	template<typename Archive>
	class SaveTupleCallback {
	public:
		Archive* m_ar;

		SaveTupleCallback(const Archive& ar) : m_ar((Archive*)&ar) {};
		template<typename T>
		void operator()(int index, T& ref) {
			m_ar->push(ref);	
		}
	};

	template<typename ... Args, typename Archive>
	void save(Tuple<Args ...>& ref, Archive& ar) {
		SaveTupleCallback<Archive> callback(ar);
		gxx::tuple_for_each(ref, callback);		
	};

	template<typename Archive>
	class LoadTupleCallback {
	public:
		Archive* m_ar;

		LoadTupleCallback(const Archive& ar) : m_ar((Archive*)&ar) {};
		template<typename T>
		void operator()(int index, T& ref) {
			m_ar->pull(ref);	
		}
	};

	template<typename ... Args, typename Archive>
	void load(Tuple<Args ...>& ref, Archive& ar) {
		LoadTupleCallback<Archive> callback(ar);
		gxx::tuple_for_each(ref, callback);		
	};


}

/*template<typename T, typename Archive>
inline void operator<<(T& ref, Archive& ar) {
	ar.pull(ref);	
}

template<typename T, typename Archive>
inline void operator>>(T& ref, Archive& ar) {
	ar.push(ref);	
}*/


#endif 