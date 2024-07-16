#ifndef RALGO_SPECTRE_FFT_H
#define RALGO_SPECTRE_FFT_H

/**
    @file
*/

#include <complex>
#include <stdlib.h>
#include <vector>

//#include <nos/print.h>
#include <igris/math/defs.h>
#include <ralgo/linalg/vecops.h>

namespace ralgo
{
    namespace signal
    {
        // Простая рекурсивная реализация fft.
        void fft_reference(std::vector<std::complex<double>> &a, bool invert);

        void spectre(double *AVal, double *FTvl, int Nvl, int Nft);

        std::vector<double> spectre(double *src, size_t srclen, size_t dstlen);

        std::vector<double> rfft(const std::vector<double> &src);

        /* Write array of freqs for spectre or other fft operations to buffer
         *
         * dst - target array
         * n - size of target array
         * delta - time between samples
         */
        void fftfreq(double *dst, size_t n, double delta);

        /* Get array of freqs for spectre or other fft operations
         *
         * n - size of target array
         * delta - time between samples
         */
        std::vector<double> fftfreq(size_t n, double delta);

        void rfftfreq(double *dst, size_t n, double delta);
        std::vector<double> rfftfreq(size_t n, double delta);

        // Cooley–Tukey FFT (in-place, divide-and-conquer)
        // Higher memory requirements and redundancy although more intuitive
        // Вторая референсная реализация БПФ
        template <class CArray> void fft_reference2(CArray &x)
        {
            using B = typename CArray::value_type;

            const size_t N = x.size();

            if (N <= 1)
                return;

            // divide
            CArray even = ralgo::vecops::slice(x, 0, N / 2, 2);
            CArray odd = ralgo::vecops::slice(x, 1, N / 2, 2);

            // conquer
            fft_reference2(even);
            fft_reference2(odd);

            // combine
            for (size_t k = 0; k < N / 2; ++k)
            {
                B t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
                x[k] = even[k] + t;
                x[k + N / 2] = even[k] - t;
            }
        }

        // Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
        // Better optimized but less intuitive
        // Боевая реализация
        template <class CArray> void fft(CArray &x)
        {
            using Complex = typename CArray::value_type;

            // DFT
            unsigned int N = x.size(), k = N, n;
            double thetaT = 3.14159265358979323846264338328L / N;
            Complex phiT = Complex(::cos(thetaT), -::sin(thetaT)), T;

            while (k > 1)
            {
                n = k;
                k >>= 1;
                phiT = phiT * phiT;
                T = 1.0L;

                for (unsigned int l = 0; l < k; l++)
                {
                    for (unsigned int a = l; a < N; a += n)
                    {
                        unsigned int b = a + k;
                        Complex t = x[a] - x[b];
                        x[a] += x[b];
                        x[b] = t * T;
                    }

                    T *= phiT;
                }
            }

            // Decimate
            unsigned int m = (unsigned int)log2(N);

            for (unsigned int a = 0; a < N; a++)
            {
                uint32_t b = a;
                // Reverse bits
                b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
                b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
                b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
                b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
                b = ((b >> 16) | (b << 16)) >> (32 - m);

                if (b > a)
                {
                    Complex t = x[a];
                    x[a] = x[b];
                    x[b] = t;
                }
            }

            //// Normalize (This section make it not working correctly)
            // Complex f = 1.0 / sqrt(N);
            // for (unsigned int i = 0; i < N; i++)
            //	x[i] *= f;
        }

        // inverse fft (in-place)
        template <class CArray> void ifft(CArray &x)
        {
            // conjugate the complex numbers
            ralgo::vecops::inplace::conj(x);

            // forward fft
            fft(x);

            // conjugate the complex numbers again
            ralgo::vecops::inplace::conj(x);

            // scale the numbers
            ralgo::vecops::inplace::div(x, x.size());
        }
    }
}

#endif
