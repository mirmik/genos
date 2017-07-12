#ifndef GENOS_MATH_VECTOR_H
#define GENOS_MATH_VECTOR_H

namespace gmath {
	
	template <typename T>
	class Vector3 {
		
		union {
			T data[3];
			struct {
				T x;
				T y;
				T z;
			};
		};

	public:
		Vector3() : x(), y(), z() {};
		Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {};

		Vector3& operator+ (const Vector3& b) {
			return Vector3(x+b.x, y+b.y, z+b.z);
		};

		gxx::string to_str() const {
			gxx::string str;
			str.reserve(128);
			str << '{' << x << ' ' << y << ' ' << z << '}'; 
			return gxx::move(str.shrink_to_print());
		}
	};

} 

#endif