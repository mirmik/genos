#include <math.h>
#include <ralgo/signal/fft.h>

#include <nos/print.h>

// src - массив анализируемых данных, srclen - длина массива должна быть кратна
// степени 2. dst - массив полученных значений, dstlen - длина массива должна
// быть равна srclen.

const double TwoPi = 6.283185307179586;

void ralgo::signal::fft_reference(std::vector<std::complex<double>> &a,
                                  bool invert)
{
    int n = (int)a.size();
    if (n == 1)
        return;

    std::vector<std::complex<double>> a0(n / 2), a1(n / 2);

    for (int i = 0, j = 0; i < n; i += 2, ++j)
    {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }

    fft_reference(a0, invert);
    fft_reference(a1, invert);

    double ang = 2 * M_PI / n * (invert ? -1 : 1);

    std::complex<double> w(1), wn(cos(ang), sin(ang));

    for (int i = 0; i < n / 2; ++i)
    {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert)
            a[i] /= 2, a[i + n / 2] /= 2;
        w *= wn;
    }
}

void ralgo::signal::spectre(double *src, double *dst, int srclen, int dstlen)
{
    int i, j, n, m, Mmax, Istp;
    double Tmpr, Tmpi, Wtmp, Theta;
    double Wpr, Wpi, Wr, Wi;
    double *Tmvl;

    n = srclen * 2;
    Tmvl = new double[n];

    for (i = 0; i < n; i += 2)
    {
        Tmvl[i] = 0;
        Tmvl[i + 1] = src[i / 2];
    }

    i = 1;
    j = 1;

    while (i < n)
    {
        if (j > i)
        {
            Tmpr = Tmvl[i];
            Tmvl[i] = Tmvl[j];
            Tmvl[j] = Tmpr;
            Tmpr = Tmvl[i + 1];
            Tmvl[i + 1] = Tmvl[j + 1];
            Tmvl[j + 1] = Tmpr;
        }

        i = i + 2;
        m = srclen;

        while ((m >= 2) && (j > m))
        {
            j = j - m;
            m = m >> 1;
        }

        j = j + m;
    }

    Mmax = 2;

    while (n > Mmax)
    {
        Theta = -TwoPi / Mmax;
        Wpi = sin(Theta);
        Wtmp = sin(Theta / 2);
        Wpr = Wtmp * Wtmp * 2;
        Istp = Mmax * 2;
        Wr = 1;
        Wi = 0;
        m = 1;

        while (m < Mmax)
        {
            i = m;
            m = m + 2;
            Tmpr = Wr;
            Tmpi = Wi;
            Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
            Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;

            while (i < n)
            {
                j = i + Mmax;
                Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j - 1];
                Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j - 1];

                Tmvl[j] = Tmvl[i] - Tmpr;
                Tmvl[j - 1] = Tmvl[i - 1] - Tmpi;
                Tmvl[i] = Tmvl[i] + Tmpr;
                Tmvl[i - 1] = Tmvl[i - 1] + Tmpi;
                i = i + Istp;
            }
        }

        Mmax = Istp;
    }

    for (i = 0; i < dstlen; i++)
    {
        j = i * 2;
        dst[i] = 2 * sqrt(pow(Tmvl[j], 2) + pow(Tmvl[j + 1], 2)) / srclen;
    }

    delete[] Tmvl;
}

std::vector<double>
ralgo::signal::spectre(double *src, size_t srclen, size_t dstlen)
{
    std::vector<double> v(dstlen);
    ralgo::signal::spectre(src, v.data(), srclen, dstlen);
    return v;
}

void ralgo::signal::fftfreq(double *dst, size_t n, double delta)
{
    int _n = n;

    for (int i = 0; i <= (_n / 2) - 1; ++i)
    {
        dst[i] = i / (delta * n);
    }
    for (int i = -(_n / 2); i <= -1; ++i)
    {
        dst[n + i] = i / (delta * n);
    }
}

void ralgo::signal::rfftfreq(double *dst, size_t n, double delta)
{
    for (unsigned int i = 0; i <= n / 2; ++i)
    {
        dst[i] = i / (delta * n);
    }
}

std::vector<double> ralgo::signal::fftfreq(size_t n, double delta)
{
    std::vector<double> v(n);
    ralgo::signal::fftfreq(v.data(), n, delta);
    return v;
}

std::vector<double> ralgo::signal::rfftfreq(size_t n, double delta)
{
    std::vector<double> v((n / 2) + 1);
    ralgo::signal::rfftfreq(v.data(), n, delta);
    return v;
}

std::vector<double> ralgo::signal::rfft(const std::vector<double> &src)
{
    std::vector<double> don;
    std::vector<double> res;

    int size = src.size();
    bool is_biexp = (size & (size - 1)) == 0;

    if (!is_biexp)
    {
        int clz = __builtin_clz(size);
        size = (1 << (sizeof(int) * 8 - clz));
    }

    don.resize(size);
    res.resize(size);

    std::copy(src.begin(), src.end(), don.begin());

    spectre(don.data(), res.data(), don.size(), res.size());
    return res;
}
