#ifndef UTILXX_TRY_H
#define UTILXX_TRY_H

namespace mtry {
	template<typename T> 
	struct abstract {
		T data;
		abstract(T d) : data(d) {}
		inline void move(abstract& other) {	data = std::move(other.data); } 
		inline void destructor() { data.~T(); }
		operator T() { return data; }
	};
	
	template<typename T> 
	struct abstract<T&> {
		T& data;
		abstract(T& d) : data(d) {};
		inline void move(abstract& other) { data = other.data; } 
		inline void destructor() {}
		operator T&() {return data;}
	};
	
	template<typename T> 
	struct abstract<T&&> {
		T&& data;
		abstract(T& d) : data(std::move(d)) {};
		abstract(T&& d) : data(std::move(d)) {};
		inline void move(abstract& other) { data = other.data; } 
		inline void destructor() {}
		operator T&&() {return std::move(data);}
	};
		
	struct error {
		int8_t code;
		std::string info;
		explicit error(int8_t c) : code(c) {}
		explicit error(int8_t c, std::string str) : code(c), info(str) {}
		error(error&& e) : code(e.code), info(std::move(e.info)) {}; 
	
		error& operator=(error&& res) = default;
		error() = default;
	};
	
	template <typename T, typename E = error>
	struct result {
		using Result = T;
		uint8_t iserror;
		union {
			abstract<T> data;
			E error;
		};
		
		template<typename U>
		result(U&& r) : iserror(false), data(std::forward<U>(r)) {}
		
		result(E&& e) : iserror(true), error(std::forward<E>(e)) {}
		
		result(result&& res) : iserror(res.iserror) {
			if (iserror) { 
				data.move(res.data);
			} else { 
				error = std::move(res.error);
			}; 
			iserror = 2;
		}
		
		~result() {
			switch (iserror) {
				case 1: error.~E(); break;
				case 0: data.destructor(); break;
				default: break;	
			};		
		}
	
		inline void restore(T&& r) {
			iserror = false;
			error.~error();
			data = std::forward<T>(r);	
		}
	
		inline void restore() {
			iserror = false;
			error.~error();
			data = Result();	
		}
	
		operator bool() {
			return iserror;
		}
	};
	
	template <typename E>
	struct result<void, E> {
		using Result = void;
		uint8_t iserror;
		uint8_t& data = iserror;
		E error;
		
		result(E&& e) : iserror(true), error(std::forward<E>(e)) {}
		
		result(result&& res) : 
			iserror(2), 
			error(std::move(res.error)) 
		{}
		
		result() : iserror(false) {};
		result(const result& res) = delete;
		
		void operator=(result&& res) = delete;
		void operator=(const result& res) = delete;
		
		~result() {
			printf("~result");
			switch (iserror) {
				case 1: error.~E(); break;
				default: break;	
			};		
		}
	
		inline void restore() {
			iserror = false;
			error.~error();
		}
	
		operator bool() {
			return iserror;
		}
	};
}

#define tryS(invoke) ({												\
	auto&& __result = (invoke); 									\
	if (__result.iserror) return std::move(__result.error);		\
	std::move(__result.data);									\
}) 

#define tryH(invoke,err,handler) ({									\
	__label__ try_label;											\
	auto&& __result = (invoke);									\
	if (__result.iserror) {										\
		auto&& err = __result.error; 								\
		handler; 													\
		return std::move(__result.error);							\
	}; 																\
	try_label:														\
	__result.data;												\
})

#define tryP(invoke,err,handler) ({									\
	__label__ try_label;											\
	auto&& __result = (invoke);									\
	if (__result.iserror) {										\
		auto&& err = __result.error; 								\
		handler; 													\
		__result.restore();										\
	}; 																\
	try_label:														\
	__result.data;												\
})

#define try_return(val) ({__result.restore(val); goto try_label;})

#endif