#ifndef RABBIT_ELLIPSOIDE_FITTING_H
#define RABBIT_ELLIPSOIDE_FITTING_H

#include <ralgo/linalg/svd.h>
#include <rabbit/types.h>
#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/matrix_view.h>
#include <ralgo/linalg/solve.h>

#include <ralgo/linalg/svd.h>

namespace rabbit
{
	template <class T, class Allocator = std::allocator<real>>
	class ellipse_fit
	{
		ralgo::matrix_view_ro<T> data;
		ralgo::matrix<T, ralgo::row_order<T>, Allocator> _D;
		std::vector<T> _B;
		std::vector<T> _X;
		std::vector<linalg::vec<T, 2>> _points;
		std::vector<linalg::vec<T, 2>> _points_off;
		linalg::vec<T, 2> _center;

		ralgo::matrix<T, ralgo::row_order<T>, Allocator> _canonical;

	public:
		ellipse_fit(const std::vector<linalg::vec<T, 2>> & points)
			:
			_points(points)
		{
			data = ralgo::matrix_view_ro<real>((real*)&_points[0], _points.size(), 2);
			_D.resize(_points.size(), 5);
			_B.resize(_points.size());
			_X.resize(5);

			init();
			solve();
		}

		void init()
		{
			for (int i = 0; i < _D.rows(); ++i)
			{
				auto x = data.row(i)[0];
				auto y = data.row(i)[1];

				auto x_sq = x * x;
				auto y_sq = y * y;

				_D.at(i, 0) = x_sq;
				_D.at(i, 1) = y_sq;
				_D.at(i, 2) = x * y;
				_D.at(i, 3) = x;
				_D.at(i, 4) = y;

				_B[i] = 1;//x_sq + y_sq + z_sq;
			}
		}

		void solve()
		{
			_X = ralgo::solve(_D, _B);

			auto a = _X[0];
			auto b = _X[1];
			auto c = _X[2];
			auto d = _X[3];
			auto e = _X[4];

			_center =
			{
				(-2 * b * d + c * e) / (4 * a * b - c * c),
				(-2 * a * e + c * d) / (4 * a * b - c * c)
			};

			_points_off = _points;
			for (int i = 0; i < _points_off.size(); ++i)
				_points[i] = _points[i] - _center;

			init();
			_X = ralgo::solve(_D, _B);

			{
				auto a = _X[0];
				auto b = _X[1];
				auto c = _X[2];
				auto d = _X[3];
				auto e = _X[4];

				float m[4] = { a, c, c, b };
				ralgo::matrix_view_ro<float> mmm(m, 2, 2);

				nos::print_matrix(mmm);

				auto svd_mmm = ralgo::svd(mmm);
			}
		}

		std::vector<real> & result() { return _X; }
		linalg::vec<T, 2> center() { return _center; }
	};
}

#endif