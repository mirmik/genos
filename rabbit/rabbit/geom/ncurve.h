#ifndef GXX_GEOM_NCURVE_H
#define GXX_GEOM_NCURVE_H

#include <cmath>
#include <rabbit/geom/ngeom.h>
#include <malgo/oldfunc.h>

namespace rabbit
{
	namespace ngeom
	{
		class curve
		{
		public:
			virtual point d0(double t) const = 0;
			virtual std::vector<point> points_with(int num, double coord) const { BUG(); }
		};

		class bounded_curve : public curve
		{
		public:
			double tmin;
			double tmax;
			bounded_curve() {};
			bounded_curve(double tmin, double tmax) : tmin(tmin), tmax(tmax) {};
		};

		class line;

		class infinity_line : public curve
		{
		public:
			point l;
			direction d;

			CONSTREF_GETTER(loc, l);
			CONSTREF_GETTER(dir, d);

			/*infinity_line(const std::initializer_list<double>& loc, const std::initializer_list<double>& dir)
				: storage(2*loc.size()), curve(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()), n(loc.size()) {
				assert(loc.size() == dir.size());
				double* ptr = storage.begin();
				for (auto& f : loc) { *ptr++ = f; }
				for (auto& f : dir) { *ptr++ = f; }
			}

			vector evaluate_point(int n, double c) {
				//double spd = direction()[n];
				//assert(std::fabs(spd) > E);
			//
				//double refcoord = location()[n];
				//double swift = c - refcoord;
				//
				//vector vec(location() + direction().scale(swift));
				//return vec;
			}

			infinity_line(const line& l);*/
		};

		class line : public bounded_curve
		{
		public:
			point a;
			point b;

			line(const point& a, const point& b) : bounded_curve(0, 1), a(a), b(b) {}
			//line_segment(std::initializer_list<double> a, gxx::objbuf<double> b) : a(a), b(b), bounded_curve(0,1) {}

			size_t dim() const { return a.dim(); }

			point& pnt1() { return a; }
			point& pnt2() { return b; }
			const point& pnt1() const { return a; }
			const point& pnt2() const { return b; }

			point d0(double t) const override
			{
				return ngeom::linear_interpolation_2point(pnt1(), pnt2(), t);
			}

			std::vector<point> points_with(int n, double c) const override
			{
				double diffn = b[n] - a[n];
				if (std::fabs(diffn) < E)
				{
					return std::vector<point>();
				}


				double refcoord = a[n];
				double koeff = (c - refcoord) / diffn;


				point ret = ngeom::linear_interpolation_2point(a, b, koeff);

				return std::vector<point> {ret};
			}

			/*line(const std::initializer_list<double>& a, const std::initializer_list<double>& b) : storage(2*a.size()), curve(0, 1), n(a.size()) {
				assert(a.size() == b.size());
				double* ptr = storage.begin();
				for (auto& f : a) { *ptr++ = f; }
				for (auto& f : b) { *ptr++ = f; }
			}

			template <typename A, typename B>
			line(const A& a, const B& b) : storage(2*a.size()), curve(0, 1), n(a.size()) {
				assert(a.size() == b.size());
				double* ptr = storage.begin();
				for (auto& f : a) { *ptr++ = f; }
				for (auto& f : b) { *ptr++ = f; }
			}

			line(line&&) = delete;
			line(const line&) = delete;

			double length() {
				double sum = 0, a, b;
				for (int i = 0; i < n; ++i) {
					a = storage[i];
					b = storage[i+n];
					sum += (b - a) * (b - a);
				}
				return sqrt(sum);
			}

			infinity_line to_infinity_line() const {
				return infinity_line(*this);
			}
			*/
			size_t print_to(nos::ostream& o) const
			{
				return nos::fprint("ls({}, {})", a, b);
			}
		};

		class multiline : public bounded_curve
		{
		public:
			oldmalgo::matrix<double> mat;

		public:
			size_t dim() const
			{
				return mat.size2();
			}

			multiline(size_t n, size_t m) : bounded_curve(0, n), mat(n, m) {}

		public:

			multiline(const point& a, const point& b) : multiline(2, a.dim())
			{
				mat.row_proxy(0) = a;
				mat.row_proxy(1) = b;
			}

			multiline(const std::initializer_list<point>& pnts) 
				: multiline(pnts.size(), pnts.begin()->size())
			{
				//template <template<class> class C>
				//multiline(gxx::concept::sequence_container<C<double>,double> pnts) : multiline(pnts.size(), pnts.begin()->size()) {
				int i = 0;
				for (const auto& p : pnts)
				{
					assert(mat.size2() == p.size());
					mat.row_view(i++) = p;
				}
			}

			//multiline(size_t n, size_t m) : raw(n*m), dm(m), sz(n), bounded_curve(0,m) {}
			multiline() = default;
			multiline(const multiline&) = default;
			multiline& operator=(const multiline&) = default;

			multiline(multiline&&) = default;
			multiline& operator=(multiline&&) = default;

			size_t size() const { return mat.size1(); }

			point d0(double t) const override
			{
				BUG();
			}

			double* point_data(size_t i)
			{
				return mat.row_view(i).data();
			}

			size_t printTo(nos::ostream& o) const
			{
				return nos::print_to(o, mat);
			}
		};
	}
}

#endif
