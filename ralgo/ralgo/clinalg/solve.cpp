#include <ralgo/clinalg/solve.h>
#include <ralgo/linalg/plud.h>
#include <vector>

void linalg_square_solve_d(double *A, int n, double *b, double *x)
{
    std::vector<double> P(n * n);
    std::vector<double> L(n * n);
    std::vector<double> U(n * n);

    ralgo::matrix_view<double> A_view(A, n, n);
    ralgo::matrix_view<double> P_view(P.data(), n, n);
    ralgo::matrix_view<double> L_view(L.data(), n, n);
    ralgo::matrix_view<double> U_view(U.data(), n, n);

    ralgo::vector_view<double> b_view(b, n);
    ralgo::vector_view<double> x_view(x, n);

    auto plud_decomposition = ralgo::plud(A_view, P_view, L_view, U_view);
    plud_decomposition.solve(b_view, x_view);
}
