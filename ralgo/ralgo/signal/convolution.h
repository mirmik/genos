#ifndef RALGO_CONVOLUTION_H
#define RALGO_CONVOLUTION_H

/**
    @file
*/

#include <igris/util/bug.h>
#include <ralgo/interpolate.h>
#include <ralgo/signal/signal.h>

#include <assert.h>

namespace ralgo
{
    namespace signal
    {
        class window
        {
        public:
            double strt;
            double fini;

        public:
            window(double strt, double fini) : strt(strt), fini(fini) {}

            // virtual double convolution(double * vals, double * stamp, int n);
            // virtual double convolution(float * vals, float * stamp, int n);

            // void map(double * vals, double * stamp, int n);
        };

        class triangle_window : public window
        {
        public:
            triangle_window(double strt, double fini) : window(strt, fini) {}

            std::vector<double> values()
            {
                return {0, 1, 0};
            }
            std::vector<double> keypoints()
            {
                return {strt, (strt + fini) / 2, fini};
            }

            template <class V> std::vector<double> keypoints_map(const V &vec)
            {
                std::vector<double> r;
                std::vector<double> kpoints = keypoints();
                ralgo::vecops::merge_sorted(
                    vec, kpoints, std::back_inserter(r), strt, fini);
                return r;
            }

            auto lerp_values(const std::vector<double> &wided_keys)
            {
                std::vector<double> vals = values();
                std::vector<double> keys = keypoints();
                return ralgo::signal::lerp_values<std::vector<double>>(
                    vals, keys, wided_keys);
            }
        };

        static inline double convolution(
            double t0, double t1, double f0, double f1, double g0, double g1)
        {
            double f = f1 - f0;
            double g = g1 - g0;
            double t = t1 - t0;

            return (f0 * g0 * t + f0 * g * t / 2 + f * g0 * t / 2 +
                    f * g * t / 3);
        }

        template <class K, class V, class W>
        double convolution(const K &k, const V &v, const W &w)
        {
            double acc = 0;

            assert(k.size() == v.size());
            assert(k.size() == w.size());

            for (int i = 0; i < k.size() - 1; ++i)
            {
                double a =
                    convolution(k[i], k[i + 1], v[i], v[i + 1], w[i], w[i + 1]);
                acc += a;
            }

            return acc;
        }
    }

    template <class K, class V> double trapz(const K &k, const V &v)
    {
        double acc = 0;
        assert(k.size() == v.size());

        for (int i = 0; i < k.size() - 1; ++i)
        {
            double t = k[i + 1] - k[i];
            acc += v[i] * t + (v[i + 1] - v[i]) * t / 2;
        }

        return acc;
    }
}

#endif
