#ifndef RALGO_SVD_H
#define RALGO_SVD_H

/**
    @file
*/

#include <limits>
#include <vector>

#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/proxy.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/log.h>
#include <ralgo/util/math.h>

#include <nos/print.h>

// Based on numerical recipes.
// Attention Row Order.

// A = U W V*

#ifndef __MAX__
#define __MAX__(a, b) ((a) > (b) ? (a) : (b))
#endif

namespace ralgo
{
    template <class T, class MU, class MV, class V> struct SVD
    {
        int m = 0, n = 0;
        MU u = {};
        MV v = {};
        V w = {};
        std::vector<double> mean_of_columns = {};
        T eps = {};
        T tsh = {};

        template <class MA>
        SVD(const MA &_a, const MU &_u, const MV &_v, const V &_w)
            : m(_a.size1()), n(_a.size2()), // берём размерность.
              u(_u), v(_v), w(_w)
        {
            u.resize(_a.rows(), _a.cols());
            v.resize(_a.cols(), _a.cols());
            w.resize(_a.cols());
            mean_of_columns.resize(_a.cols());

            ralgo::matops::copy(u, _a); // Копируем данные.
            ralgo::vecops::inplace::clean(v);
            ralgo::vecops::inplace::clean(w);
            ralgo::vecops::fill(mean_of_columns, 0);

            for (int i = 0; i < n; ++i)
            {
                T sum = 0;
                for (int j = 0; j < m; ++j) {
                    sum += u(j, i);
                }
                mean_of_columns[i] = (double)sum / (double)m;
            }

            eps = std::numeric_limits<T>::epsilon();

            decompose();
            reorder();
            tsh = 0.5 * sqrt(m + n + 1.) * w[0] * eps;
        }

        template <class MA> SVD(const MA &a) : SVD(a, MU{}, MV{}, V{}) {}

        template <class B, class X>
        void solve(const B &b, X &x, T thresh = -1.);

        template <class R> void pinv(R &result)
        {
            // T u_inv_buffer [u.rows() * u.cols()];
            // T v_inv_buffer [v.rows() * v.cols()];
            // T w_inv_buffer [w.size()];

            // ralgo::matrix_view<T> u_inv(u_inv_buffer, u.cols(), u.rows());
            // ralgo::matrix_view<T> v_inv(v_inv_buffer, v.cols(), v.rows());
            // ralgo::vector_view<T> w_inv(w_inv_buffer, w.size());

            // ralgo::transpose(u, u_inv);
            // ralgo::transpose(v, v_inv);

            ralgo::transposed_matrix_proxy u_inv(u);
            ralgo::transposed_matrix_proxy v_inv(v);
            ralgo::inverted_diagonal_proxy w_inv(w);

            ralgo::matops::multiply(u_inv, w_inv, result);
        }

        ralgo::matrix<T> pinv()
        {
            ralgo::matrix<T> x;
            pinv(x);
            return x;
        }

        void decompose();
        void reorder();

        T pythag(const T a, const T b)
        {
            T absa = std::abs(a), absb = std::abs(b);
            return (absa > absb
                        ? absa * sqrt(1.0 + ralgo::sqr(absb / absa))
                        : (absb == 0.0
                               ? 0.0
                               : absb * sqrt(1.0 + ralgo::sqr(absa / absb))));
        }
    };

    template <class T, class MU, class MV, class V>
    template <class A, class B>
    void SVD<T, MU, MV, V>::solve(const A &b, B &x, T thresh)
    {
        int i, j, jj;
        T s;
        if (std::size(b) != (unsigned)m || ralgo::size(x) != (unsigned)n)
            ralgo::fault("SVD::solve bad sizes");

        TEMPORARY_STORAGE(T, n, tmp);
        tsh = (thresh >= 0. ? thresh : 0.5 * sqrt(m + n + 1.) * w[0] * eps);
        for (j = 0; j < n; j++)
        {
            s = 0.0;
            if (w[j] > tsh)
            {
                for (i = 0; i < m; i++)
                    s += u[i][j] * b[i];
                s /= w[j];
                tmp[j] = s;
            }
            else  {
                tmp[j] = 0;
            }           
        }
        for (j = 0; j < n; j++)
        {
            s = 0.0;
            for (jj = 0; jj < n; jj++) {
                s += v[j][jj] * tmp[jj];
            }
            x[j] = s;
        }
    }
    /*
        template <typename M>
        int SVD<M>::rank(type_t<M> thresh)
        {
            int j, nr = 0;
            tsh = (thresh >= 0. ? thresh : 0.5 * sqrt(m + n + 1.) * w[0] * eps);
            for (j = 0; j < n; j++) if (w[j] > tsh) nr++;
            return nr;
        }

        template <typename M>
        int SVD<M>::nullity(type_t<M> thresh)
        {
            int j, nn = 0;
            tsh = (thresh >= 0. ? thresh : 0.5 * sqrt(m + n + 1.) * w[0] * eps);
            for (j = 0; j < n; j++) if (w[j] <= tsh) nn++;
            return nn;
        }

        template <typename M>
        matrix_t<M> SVD<M>::range(type_t<M> thresh)
        {
            int i, j, nr = 0;
            matrix_t<M> rnge(m, rank(thresh));
            for (j = 0; j < n; j++)
            {
                if (w[j] > tsh)
                {
                    for (i = 0; i < m; i++) rnge[i][nr] = u[i][j];
                    nr++;
                }
            }
            return rnge;
        }

        template <typename M>
        matrix_t<M> SVD<M>::nullspace(type_t<M> thresh)
        {
            int j, jj, nn = 0;
            matrix_t<M> nullsp(n, nullity(thresh));
            for (j = 0; j < n; j++)
            {
                if (w[j] <= tsh)
                {
                    for (jj = 0; jj < n; jj++) nullsp[jj][nn] = v[jj][j];
                    nn++;
                }
            }
            return nullsp;
        }
        */
    template <class T, class MU, class MV, class V>
    void SVD<T, MU, MV, V>::decompose()
    {
        bool flag;
        int i, its, j, jj, k, l, nm;
        T anorm, c, f, g, h, s, scale, x, y, z;
        // vector_t<M> rv1(n);

        TEMPORARY_STORAGE(T, n, rv1);

        g = scale = anorm = 0.0;
        for (i = 0; i < n; i++)
        {
            l = i + 2;
            rv1[i] = scale * g;
            g = s = scale = 0.0;
            if (i < m)
            {
                for (k = i; k < m; k++)
                    scale += std::abs(u[k][i]);
                if (scale != 0.0)
                {
                    for (k = i; k < m; k++)
                    {
                        u[k][i] /= scale;
                        s += u[k][i] * u[k][i];
                    }
                    f = u[i][i];
                    g = -ralgo::sign(sqrt(s), f);
                    h = f * g - s;
                    u[i][i] = f - g;
                    for (j = l - 1; j < n; j++)
                    {
                        for (s = 0.0, k = i; k < m; k++)
                            s += u[k][i] * u[k][j];
                        f = s / h;
                        for (k = i; k < m; k++)
                            u[k][j] += f * u[k][i];
                    }
                    for (k = i; k < m; k++)
                        u[k][i] *= scale;
                }
            }
            w[i] = scale * g;
            g = s = scale = 0.0;
            if (i + 1 <= m && i + 1 != n)
            {
                for (k = l - 1; k < n; k++)
                    scale += std::abs(u[i][k]);
                if (scale != 0.0)
                {
                    for (k = l - 1; k < n; k++)
                    {
                        u[i][k] /= scale;
                        s += u[i][k] * u[i][k];
                    }
                    f = u[i][l - 1];
                    g = -ralgo::sign(sqrt(s), f);
                    h = f * g - s;
                    u[i][l - 1] = f - g;
                    for (k = l - 1; k < n; k++)
                        rv1[k] = u[i][k] / h;
                    for (j = l - 1; j < m; j++)
                    {
                        for (s = 0.0, k = l - 1; k < n; k++)
                            s += u[j][k] * u[i][k];
                        for (k = l - 1; k < n; k++)
                            u[j][k] += s * rv1[k];
                    }
                    for (k = l - 1; k < n; k++)
                        u[i][k] *= scale;
                }
            }
            anorm = __MAX__(anorm, (std::abs(w[i]) + std::abs(rv1[i])));
        }

        for (i = n - 1; i >= 0; i--)
        {
            if (i < n - 1)
            {
                if (g != 0.0)
                {
                    for (j = l; j < n; j++)
                        v[j][i] = (u[i][j] / u[i][l]) / g;
                    for (j = l; j < n; j++)
                    {
                        for (s = 0.0, k = l; k < n; k++)
                            s += u[i][k] * v[k][j];
                        for (k = l; k < n; k++)
                            v[k][j] += s * v[k][i];
                    }
                }
                for (j = l; j < n; j++)
                    v[i][j] = v[j][i] = 0.0;
            }
            v[i][i] = 1.0;
            g = rv1[i];
            l = i;
        }

        for (i = ralgo::min(m, n) - 1; i >= 0; i--)
        {
            l = i + 1;
            g = w[i];
            for (j = l; j < n; j++)
                u[i][j] = 0.0;
            if (g != 0.0)
            {
                g = 1.0 / g;
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < m; k++)
                        s += u[k][i] * u[k][j];
                    f = (s / u[i][i]) * g;
                    for (k = i; k < m; k++)
                        u[k][j] += f * u[k][i];
                }
                for (j = i; j < m; j++)
                    u[j][i] *= g;
            }
            else
                for (j = i; j < m; j++)
                    u[j][i] = 0.0;
            ++u[i][i];
        }
        for (k = n - 1; k >= 0; k--)
        {
            for (its = 0; its < 30; its++)
            {
                flag = true;
                for (l = k; l >= 0; l--)
                {
                    nm = l - 1;
                    if (l == 0 || std::abs(rv1[l]) <= eps * anorm)
                    {
                        flag = false;
                        break;
                    }
                    if (std::abs(w[nm]) <= eps * anorm)
                        break;
                }
                if (flag)
                {
                    c = 0.0;
                    s = 1.0;
                    for (i = l; i < k + 1; i++)
                    {
                        f = s * rv1[i];
                        rv1[i] = c * rv1[i];
                        if (std::abs(f) <= eps * anorm)
                            break;
                        g = w[i];
                        h = pythag(f, g);
                        w[i] = h;
                        h = 1.0 / h;
                        c = g * h;
                        s = -f * h;
                        for (j = 0; j < m; j++)
                        {
                            y = u[j][nm];
                            z = u[j][i];
                            u[j][nm] = y * c + z * s;
                            u[j][i] = z * c - y * s;
                        }
                    }
                }
                z = w[k];
                if (l == k)
                {
                    if (z < 0.0)
                    {
                        w[k] = -z;
                        for (j = 0; j < n; j++)
                            v[j][k] = -v[j][k];
                    }
                    break;
                }
                if (its == 29)
                    ralgo::fault("no convergence in 30 svdcmp iterations");
                x = w[l];
                nm = k - 1;
                y = w[nm];
                g = rv1[nm];
                h = rv1[k];
                f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
                g = pythag(f, 1.0);
                f = ((x - z) * (x + z) +
                     h * ((y / (f + ralgo::sign(g, f))) - h)) /
                    x;
                c = s = 1.0;
                for (j = l; j <= nm; j++)
                {
                    i = j + 1;
                    g = rv1[i];
                    y = w[i];
                    h = s * g;
                    g = c * g;
                    z = pythag(f, h);
                    rv1[j] = z;
                    c = f / z;
                    s = h / z;
                    f = x * c + g * s;
                    g = g * c - x * s;
                    h = y * s;
                    y *= c;
                    for (jj = 0; jj < n; jj++)
                    {
                        x = v[jj][j];
                        z = v[jj][i];
                        v[jj][j] = x * c + z * s;
                        v[jj][i] = z * c - x * s;
                    }
                    z = pythag(f, h);
                    w[j] = z;
                    if (z)
                    {
                        z = 1.0 / z;
                        c = f * z;
                        s = h * z;
                    }
                    f = c * g + s * y;
                    x = c * y - s * g;
                    for (jj = 0; jj < m; jj++)
                    {
                        y = u[jj][j];
                        z = u[jj][i];
                        u[jj][j] = y * c + z * s;
                        u[jj][i] = z * c - y * s;
                    }
                }
                rv1[l] = 0.0;
                rv1[k] = f;
                w[k] = x;
            }
        }
    }

    template <class T, class MU, class MV, class V>
    void SVD<T, MU, MV, V>::reorder()
    {
        int i, j, k, s, inc = 1;
        T sw;
        // vector_t<M> su(m), sv(n);
        // T su[m];
        TEMPORARY_STORAGE(T, m, su);
        // T sv[n];
        TEMPORARY_STORAGE(T, n, sv);

        do
        {
            inc *= 3;
            inc++;
        } while (inc <= n);

        do
        {
            inc /= 3;
            for (i = inc; i < n; i++)
            {
                sw = w[i];
                for (k = 0; k < m; k++)
                    su[k] = u[k][i];
                for (k = 0; k < n; k++)
                    sv[k] = v[k][i];
                j = i;
                while (w[j - inc] < sw)
                {
                    w[j] = w[j - inc];
                    for (k = 0; k < m; k++)
                        u[k][j] = u[k][j - inc];
                    for (k = 0; k < n; k++)
                        v[k][j] = v[k][j - inc];
                    j -= inc;
                    if (j < inc)
                        break;
                }
                w[j] = sw;
                for (k = 0; k < m; k++)
                    u[k][j] = su[k];
                for (k = 0; k < n; k++)
                    v[k][j] = sv[k];
            }
        } while (inc > 1);

        for (k = 0; k < n; k++)
        {
            s = 0;
            for (i = 0; i < m; i++)
                if (u[i][k] < 0.)
                    s++;
            for (j = 0; j < n; j++)
                if (v[j][k] < 0.)
                    s++;
            if (s > (m + n) / 2)
            {
                for (i = 0; i < m; i++)
                    u[i][k] = -u[i][k];
                for (j = 0; j < n; j++)
                    v[j][k] = -v[j][k];
            }
        }
    }

    template <class MA, class MU, class MV, class V>
    SVD<typename MA::value_type, MU, MV, V>
    make_SVD(const MA &a, MU &u, MV &v, V &w)
    {
        return SVD<typename MA::value_type, MU, MV, V>(a, u, v, w);
    }

    template <class M>
    SVD<typename M::value_type,
        ralgo::matrix<typename M::value_type,
                      ralgo::row_order<typename M::value_type>,
                      std::allocator<typename M::value_type>>,
        ralgo::matrix<typename M::value_type,
                      ralgo::row_order<typename M::value_type>,
                      std::allocator<typename M::value_type>>,
        std::vector<typename M::value_type>>
    svd(const M &a)
    {
        return SVD<typename M::value_type,
                   ralgo::matrix<typename M::value_type,
                                 ralgo::row_order<typename M::value_type>,
                                 std::allocator<typename M::value_type>>,
                   ralgo::matrix<typename M::value_type,
                                 ralgo::row_order<typename M::value_type>,
                                 std::allocator<typename M::value_type>>,
                   std::vector<typename M::value_type>>(a);
    }
}

#endif
