/** @file Графики на движке opengl */

#ifndef RABBIT_CHARTS_H
#define RABBIT_CHARTS_H

#include <ralgo/linalg/linalg.h>
#include <ralgo/signal/sliding_array.h>

namespace rabbit
{
	struct xypoint
	{
		float x;
		float y;
		xypoint(float x, float y) : x(x), y(y) {}
	};

	class chart_axis 
	{
		
	};

	class xyseries_basic
	{
	public:
		virtual void buffer(xypoint ** arr, int * siz) = 0; 
		virtual void minmax(linalg::vec<float,2> & min, linalg::vec<float,2> & max) = 0; 
	};

	class xyseries : public xyseries_basic
	{
		std::vector<xyseries> points;

		void append(const xypoint & pnt) {}
	};

	class sliding_xyseries : public xyseries_basic
	{	
		ralgo::sliding_array<xypoint> array;

		void buffer(xypoint ** arr, int * siz) 
		{
			auto view = array.window();
			*arr = (xypoint*)view.data();
			*siz = view.size();
		}
	};

	class chart 
	{
		std::vector<xyseries *> series;

	public:
		chart()
		{

		}

		void add_series(xyseries * xys) 
		{
			series.push_back(xys);
		}
	};
}

#endif