#ifndef UTILXX_TRY_H
#define UTILXX_TRY_H

#include "assert.h"

namespace mtry {
	template<typename T> 
	struct tryhelper {
		using type = T;
		using storedtype = T;
		template <typename Arg>
		static void constructor(type& ths, Arg oth) {
			new (&ths) type(std::forward<Arg>(oth));
		}
		static void destructor(type& ths) {ths.~type();};
		static void constructor(type& ths) {
			new (&ths) type();
		};
	};
	
	template<typename T> 
	struct tryhelper<T&> {
		using type = T&;
		using storedtype = std::reference_wrapper<T>;
		template <typename Arg>
		static void constructor(storedtype& ths, Arg oth) {
			new (&ths) storedtype(oth);	
		}
		static void destructor(storedtype& ths) {ths.~storedtype();};
		static void constructor(storedtype& ths) {};
	};
	
	template<typename T> 
	struct tryhelper<T&&> {
		using type = T&&;
		using storedtype = T&&;
		template <typename Arg>
		static void constructor(type& ths, Arg oth) {	
			ths = oth;
		}
		static void destructor(type& ths) {};
		static void constructor(type& ths) {};
	};
		
	struct error {
		int8_t code;
		std::string info;
		explicit error(int8_t c) : code(c), info() {}
		explicit error(int8_t c, const char* str) : code(c), info(str) {}
		explicit error(int8_t c, std::string str) : code(c), info(str) {}
		explicit error(int8_t c, std::string&& str) : code(c), info(std::move(str)) {}
		
		error(error&& e) = default;
		error& operator=(error&& other) = default;		
		~error() = default;
	};
	
	template <typename T, typename E = error>
	class result {
	public:
		using Stored = typename tryhelper<T>::storedtype;
		using Result = Stored;
		uint8_t _iserror;
		union {
			Stored _data;
			E _error;
		};
	
	public:
		
		template<typename U>
		result(U&& r) :	_iserror(false), _data(std::forward<U>(r)) {}
		result(E&& e) : _iserror(true), _error(std::forward<E>(e)) {}
		
		result(result&& res) : _iserror(res._iserror) {
			if (_iserror) {
				new (&_error) E(std::move(res._error));
			} else { 
				//new (&_data) Stored(std::move(res._data));
				tryhelper<T>::constructor(_data, std::move(res._data));
			}; 
			_iserror = 2;
		}
		
		~result() {
			switch (_iserror) {
				case 1: _error.~E(); break;
				//case 0: _data.~Stored(); break;
				case 0: tryhelper<T>::destructor(_data); break;
				default: break;	
			};		
		}
	
		inline void restore(T&& r) {
			_iserror = false;
			_error.~error();
			_data = std::forward<T>(r);	
		}
	
		Result& getData() {
			assert(_iserror == 0);
			return _data;
		}
		
		Result& getData(std::function<void(E&)> h) {
			if (_iserror == 1) {
				h(_error);
			}
			return _data;
		}		

		E& getError() {
			assert(_iserror == 1);
			return _error;
		}
		
		bool iserror() {
			return _iserror;
		}

		operator T() {
			assert(iserror == 0);
			return _data;
		}

		inline void restore() {
			assert(_iserror == 1);
			_iserror = false;
			_error.~error();
			tryhelper<T>::constructor(_data);	
		}
	
		operator bool() {
			return _iserror;
		}
	};

	template <typename E>
	struct result<void, E> {
		using Result = void;
		uint8_t _iserror;
		union {
			E _error;
		};
		
		result() : _iserror(false) {}
		result(E&& e) : _iserror(true), _error(std::move(e)) {}
		
		result(result&& res) : _iserror(res._iserror) {
			if (_iserror) {
				new (&_error) E(std::move(res._error));
			} else { }; 
			_iserror = 2;
		}
		
		~result() {
			switch (_iserror) {
				case 1: _error.~E(); break;
				default: break;	
			};		
		}
	
		void getData() {
			assert(_iserror == 0);
		}
		
		E& getError() {
			assert(_iserror == 1);
			return _error;
		}
		
		bool iserror() {
			return _iserror;
		}

		inline void restore() {
			assert(_iserror == 1);
			_iserror = false;
			_error.~error();
		}
	
		operator bool() {
			return _iserror;
		}
	};
}

#define tryS(invoke) ({												\
	auto&& __result = ({invoke;}); 									\
	if (__result.iserror()) return std::move(__result.getError());			\
	__result.getData();										\
}) 

#define tryH(invoke,err,handler) ({									\
	__label__ try_label;											\
	auto&& __result = ({invoke;});										\
	if (__result.iserror()) {											\
		auto& err = __result.getError(); 								\
		handler; 													\
		return std::move(__result.getError());							\
	}; 																\
	try_label:														\
	__result.getData();													\
})

#define tryP(invoke,err,handler) ({									\
	__label__ try_label;											\
	auto&& __result = ({invoke;});									\
	if (__result.iserror()) {											\
		auto& err = __result.getError(); 								\
		handler; 													\
		__result.restore();											\
	}; 																\
	try_label:														\
	__result._data;												\
})

#define try_restore(val) ({__result.restore(val); goto try_label;})

#endif