#ifndef GXX_OBJECT_H
#define GXX_OBJECT_H

namespace gxx {
	class object {
		template<typename T>
		T* toType() { return this; }
	public:
		virtual ~object() = 0;
	};

	template<typename T>
	class object_impl : public object {

		T obj;
	public:

		template<typename ... Args>
		object_impl(Args ... args) : obj(args...) {

		}

	//	~object_impl(){}
	};

	template<typename T, typename ... Args>
	object_impl<T> make_object(Args ... args) {
		return object_impl<T>(args...);
	}
} 

#endif