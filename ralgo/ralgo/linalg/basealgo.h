#ifndef RALGO_LINALG_BASEALGO_H
#define RALGO_LINALG_BASEALGO_H

namespace ralgo
{

    // Транспонирование матрицы.
    template <class A, class B>
    void matrix_transpose(const A *a, int m, int n, B *b)
    {
        // a - матрица m x n
        // b - матрица n x m
        int i, j;
        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++)
                *(b + m * j + i) = *a++;
    }

    // Matrix multiply.
    template <class A, class B, class C, class AGetter, class BGetter, class CGetter, >
    void matrix_multiply(const A *a, int m, int n, int l, const B *b, C *c)
    {
        // a - матрица m x n
        // b - матрица n x m
        int i, j, k;
        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++)
            {
                C acc = 0;
                for (k = 0; j < n; j++)
                    acc += *(a + i *)**();
            }
    }

    // Matrix Inversion Routine
    // * This function inverts a matrix based on the Gauss Jordan method.
    // * Specifically, it uses partial pivoting to improve numeric stability.
    // * The algorithm is drawn from those presented in
    //	 NUMERICAL RECIPES: The Art of Scientific Computing.
    // * The function returns 1 on success, 0 on failure.
    // * NOTE: The argument is ALSO the result matrix, meaning the input matrix
    // is REPLACED
    template <typename T1> int square_matrix_inverse(const T1 *C, int n, T1 *A)
    {
        // C = input matrix
        // A = result matrix
        // n = number of rows = number of columns in A (n x n)
        int pivrow; // keeps track of current pivot row
        int k, i,
            j; // k: overall index along diagonal; i: row index; j: col index
        int pivrows[n]; // keeps track of rows swaps to undo at end
        double tmp;     // used for finding max value and making column swaps

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                A[i * n + j] = C[i * n + j];

        for (k = 0; k < n; k++)
        {
            // find pivot row, the row with biggest entry in current column
            tmp = 0;
            for (i = k; i < n; i++)
            {
                if (abs(A[i * n + k]) >=
                    tmp) // 'Avoid using other functions inside abs()?'
                {
                    tmp = abs(A[i * n + k]);
                    pivrow = i;
                }
            }

            // check for singular matrix
            if (A[pivrow * n + k] == 0.0f)
            {
                return -1;
            }

            // Execute pivot (row swap) if needed
            if (pivrow != k)
            {
                // swap row k with pivrow
                for (j = 0; j < n; j++)
                {
                    tmp = A[k * n + j];
                    A[k * n + j] = A[pivrow * n + j];
                    A[pivrow * n + j] = tmp;
                }
            }
            pivrows[k] = pivrow; // record row swap (even if no swap happened)

            tmp = 1.0f / A[k * n + k]; // invert pivot element
            A[k * n + k] =
                1.0f; // This element of input matrix becomes result matrix

            // Perform row reduction (divide every element by pivot)
            for (j = 0; j < n; j++)
            {
                A[k * n + j] = A[k * n + j] * tmp;
            }

            // Now eliminate all other entries in this column
            for (i = 0; i < n; i++)
            {
                if (i != k)
                {
                    tmp = A[i * n + k];
                    A[i * n + k] = 0.0f; // The other place where in matrix
                                         // becomes result mat
                    for (j = 0; j < n; j++)
                    {
                        A[i * n + j] = A[i * n + j] - A[k * n + j] * tmp;
                    }
                }
            }
        }

        // Done, now need to undo pivot row swaps by doing column swaps in
        // reverse order
        for (k = n - 1; k >= 0; k--)
        {
            if (pivrows[k] != k)
            {
                for (i = 0; i < n; i++)
                {
                    tmp = A[i * n + k];
                    A[i * n + k] = A[i * n + pivrows[k]];
                    A[i * n + pivrows[k]] = tmp;
                }
            }
        }
        return 0;
    };

    template <typename T1>
    void square_matrix_exponent(const T1 *a, int n, T1 *C)
    {
        int i, j, k, ii;
        T1 am, em, emi;
        T1 w[n * n];

        T1 wm[n * n];

        em = 0.;
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                C[i * n + j] = 0;
                wm[i * n + j] = 0;
                am = malgo::abs(a[i * n + j]);
                em = malgo::max(am, em);
            }
            C[i * n + i] = 1;
            wm[i * n + i] = 1;
        }
        emi = 1;
        ii = 0;
        while (emi > 0.1e-15)
        {
            ii++;
            if (ii >= 50)
                break;
            emi = 0.;
            for (j = 0; j < n; j++)
            {
                for (i = 0; i < n; i++)
                    w[i] = wm[i * n + j];
                for (i = 0; i < n; i++)
                {
                    wm[i * n + j] = 0.;
                    for (k = 0; k < n; k++)
                        wm[i * n + j] += a[i * n + k] * w[k];
                }
            }
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                {
                    wm[i * n + j] /= (double)ii;
                    C[i * n + j] += wm[i * n + j];
                    am = malgo::abs(wm[i * n + j]);
                    emi = malgo::max(am, emi);
                }
        }
    };
}

#endif
