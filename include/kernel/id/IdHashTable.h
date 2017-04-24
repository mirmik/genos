#ifndef GENOS_ID_HASHTABLE_H
#define GENOS_ID_HASHTABLE_H

namespace Genos {	

	template<
		typename T, 
		typename IdType, 
		hlist_node T::* hlnk, 
		IdType T::* id, 
		int32_t HashTableSize, 
		int32_t MaxCounter,
		IdType&(*GetKey)(T&)
	>
	class IdHashTable {

		IdType counter;
		gxx::static_hashtable<HashTableSize,
			T, IdType, hlnk, GetKey> table;

	public:
		IdHashTable() {}

	public:
		IdType registry(T& obj) {
			//dprln("Registry");
			obj.*id = get_new_id();
			table.put(obj);
			return obj.*id;
		}

		T* get(IdType getid) {
			return table.get(getid);
		}

		template<typename Function>
		void foreach(Function func) {
			table.foreach(func);
		}

	private: 
		IdType get_new_id() {
			while (table.get(++counter))
				if (counter > MaxCounter) counter = 0; 
			return counter;
		}
	};

}

#endif