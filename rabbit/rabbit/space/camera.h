#ifndef RABBIT_CAMERA_H
#define RABBIT_CAMERA_H

#include <rabbit/space/htrans2.h>
#include <rabbit/prim2.h>

namespace rabbit
{
	template <class T>
	class camproj2
	{
	public:
		T koeff;

		camproj2(const T& arg)
			: koeff(arg)
		{}

		T project(linalg::vec<T,2> pnt) 
		{
			return pnt.x / pnt.y * koeff;
		}

		linalg::vec<T,2> direction_of_image(T p) 
		{
			return {p / koeff, 1};
		}
	};

	template <class T>
	class camera2 
	{
	public:
		camproj2<T> cproj;
		htrans2<T> trans;

		camera2(const T& cprojkoeff, const htrans2<T>& trans) 
			: cproj(cprojkoeff), trans(trans)  
		{}

		T project(linalg::vec<T,2> pnt) 
		{
			auto h = trans.inverse();
			auto pa = h.transform(pnt);
			return cproj.project(pa);
		}

		linalg::vec<T,2> direction_of_image(T koeff) 
		{
			auto d0 = cproj.direction_of_image(koeff);
			return trans.rotate(d0);
		}

		rabbit::line2eq<T> line_of_image(T koeff) 
		{
			auto dir = direction_of_image(koeff);
			auto line = rabbit::line2eq<T>::from_points(trans.center, trans.center + dir);

			return line; 
		}
	};
}

#endif