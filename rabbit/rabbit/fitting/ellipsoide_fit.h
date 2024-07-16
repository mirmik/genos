#ifndef RABBIT_ELLIPSOIDE_FITTING_H
#define RABBIT_ELLIPSOIDE_FITTING_H

#include <ralgo/linalg/svd.h>
#include <rabbit/types.h>
#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/matrix_view.h>
#include <ralgo/linalg/solve.h>

namespace rabbit
{
	template <class T, class Allocator = std::allocator<real>>
	class ellipsoide_fit
	{
		ralgo::matrix_view_ro<T> data;
		ralgo::matrix<T, ralgo::row_order<T>, Allocator> _D;
		std::vector<T> _B;
		std::vector<T> _X;
		std::vector<linalg::vec<T,3>> _points;

	public:
		ellipsoide_fit(const std::vector<linalg::vec<T,3>> & points)
			:
			_points(points)
		{
			data = ralgo::matrix_view_ro<real>((real*)&points[0], points.size(), 3);
			_D.resize(_points.size(), 8);
			_B.resize(_points.size());
			_X.resize(8);

			init();
			solve();
		}

		void init()
		{
			for (int i = 0; i < _D.rows(); ++i)
			{
				auto x = data.row(i)[0];
				auto y = data.row(i)[1];
				auto z = data.row(i)[2];

				auto x_sq = x * x;
				auto y_sq = y * y;
				auto z_sq = z * z;

				_D.at(i, 0) = x_sq + y_sq - 2. * z_sq;
				_D.at(i, 1) = x_sq + z_sq - 2. * y_sq;
				_D.at(i, 2) = 2. * x * y;
				_D.at(i, 3) = 2. * x * z;
				_D.at(i, 4) = 2. * y * z;
				_D.at(i, 5) = 2. * x;
				_D.at(i, 6) = 2. * y;
				_D.at(i, 7) = 2. * z;
				//_D.at(i, 8) = 1;

				_B[i] = -1;//x_sq + y_sq + z_sq;
			}
		}

		void solve()
		{
			_X = ralgo::solve(_D, _B);
		}

		std::vector<real> & result() { return _X; }
	};
}

#endif