#ifndef UTILXX_TRY_H
#define UTILXX_TRY_H

namespace mtry {
	template<typename T> 
	struct abstract {
		T data;
		abstract(T d) : data(d) {}
		//inline void move(abstract&& other) {	data = std::move(other.data); } 
		inline void destructor() { data.~T(); }
		operator T() { return data; }
	};
	
	template<typename T> 
	struct abstract<T&> {
		T& data;
		abstract(T& d) : data(d) {	std::cout << "abstract(T&)" << std::endl; };
		//inline void move(abstract&& other) { data = other.data; } 
		inline void destructor() {}
		operator T&() {return data;}
	};
	
	template<typename T> 
	struct abstract<T&&> {
		T&& data;
		abstract(T& d) : data(std::move(d)) {};
		abstract(T&& d) : data(std::move(d)) {};
		//inline void move(abstract& other) { data = other.data; } 
		inline void destructor() {}
		operator T&&() {return std::move(data);}
	};
		
	struct error {
		int8_t code;
		std::string info;
		explicit error(int8_t c) : code(c), info() {std::cout << "error(int)" << std::endl;}
		explicit error(int8_t c, std::string str) : code(c), info(str) {std::cout << "error(int,str)" << std::endl;}
		error(error&& e) : code(e.code), info(std::move(e.info)) {
			std::cout << "error(error&&)" << std::endl;
		}; 
	
		error& operator=(error&& other) {
			std::cout << "error(error&&)" << std::endl;
			code = other.code; info = std::move(other.info);
		};
		//error() = default;

		~error() {	
			std::cout << "~error()" << std::endl;
		}
	};
	
	/*struct error {
		int8_t code;
		int16_t des = 0;
		explicit error(int8_t c) : code(c) {	
			std::cout << "error(int)" << std::endl;
		}
		error(error&& e) : code(e.code) {
			std::cout << "error(error&&)" << std::endl;
		}; 
	
		error& operator=(error&& other) {
			std::cout << "=error(error&&)" << std::endl;
			code = other.code;
		};
		//error() = default;

		~error() {	
			std::cout << "~error()" << des++ << std::endl;
		}
	};*/

	template <typename T, typename E = error>
	struct result {
		using Result = T;
		uint8_t iserror;
		union {
			//abstract<T> data;
			T data;
			E error;
		};
		
		result(const T& r) : iserror(false), data(r) {
			std::cout << "result(constT&)" << std::endl;
		}
		
		result(T&& r) :	iserror(false), data(std::move(r))
		{
			std::cout << "result(T&&)" << std::endl;
		}
		
		result(E&& e) : iserror(true) {
			std::cout << "result(E&&)" << std::endl;
			error = std::move(e);
			std::cout << error.info << std::endl;
			//std::cout << e.info << std::endl;
			std::cout << "ENDresult(E&&)" << std::endl;
		}
		
		result(result&& res) : iserror(res.iserror) {
			std::cout << "result(result&&)" << std::endl;
			if (iserror) {
				error = std::move(res.error);
				std::cout << "HERE" << std::endl;
			} else { 
				data = std::move(res.data);
			}; 
			iserror = 2;
		}
		
		~result() {
			std::cout << "~result" << std::endl;
			switch (iserror) {
				//case 1: error.~E(); break;
				//case 0: data.~T(); break;
				default: break;	
			};		
		}
	
		inline void restore(T&& r) {
			iserror = false;
			error.~error();
			data = std::forward<T>(r);	
		}
	
		operator T() {
			return data;
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
	
	/*template <typename E>
	struct result<void, E> {
		using Result = void;
		uint8_t iserror;
		uint8_t& data = iserror;
		E error;
		
		result(E&& e) : iserror(true), error(std::forward<E>(e)) {
		}
		
		result(result&& res) : 
			iserror(2), 
			error(std::move(res.error)) 
		{
			printf("result(result&&)");
		}
		
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
	};*/
}

#define tryS(invoke) ({												\
	auto&& __result = ({invoke;}); 									\
			printf("HERE");										\
	if (__result.iserror) return std::move(__result.error);			\
	std::move(__result.data);										\
}) 

#define tryH(invoke,err,handler) ({									\
	__label__ try_label;											\
	auto&& __result = ({invoke;});										\
	if (__result.iserror) {											\
		auto&& err = __result.error; 								\
		handler; 													\
		return std::move(__result.error);							\
	}; 																\
	try_label:														\
	__result.data;													\
})

#define tryP(invoke,err,handler) ({									\
	__label__ try_label;											\
	auto&& __result = ({invoke;});									\
	if (__result.iserror) {											\
		auto&& err = __result.error; 								\
		handler; 													\
		__result.restore();											\
	}; 																\
	try_label:														\
	__result.data;													\
})

#define try_return(val) ({__result.restore(val); goto try_label;})

#endif