#ifndef RALGO_LINALG_SOLVE_H
#define RALGO_LINALG_SOLVE_H

#include <nos/print.h>
#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/matrix_view.h>
#include <ralgo/linalg/svd.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/linalg/vector_view.h>

namespace ralgo
{
    // b = A * x
    template <class X, class A, class B>
    void svd_linear_equation_system_solve(X &x, const A &a, const B &b)
    {
        auto m = a.size1();
        auto n = a.size2();

        assert(std::size(x) == n);
        assert(m == std::size(b));

        double u[m * n];
        double v[n * n];
        double w[n];

        ralgo::matrix_view<double> U(u, m, n);
        ralgo::matrix_view<double> V(v, n, n);
        ralgo::vector_view<double> W(w, n);

        auto svd = ralgo::make_SVD(a, U, V, W);
        svd.solve(b, x);
    }

    template <class X, class A, class B>
    void solve(X &x, const A &a, const B &b)
    {
        return svd_linear_equation_system_solve(x, a, b);
    }

    template <class X = void, class A, class B>
    auto solve(const A &a, const B &b) -> defsame_t<X, std::vector<value_t<A>>>
    {
        defsame_t<X, std::vector<value_t<A>>> x;
        x.resize(a.cols());
        solve(x, a, b);
        return x;
    }

}

#endif
