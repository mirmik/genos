#ifndef RALGO_MAGNETOMETER_H
#define RALGO_MAGNETOMETER_H

/**
    @file
*/

#include <nos/print.h>
#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    class spherical_cloud_collector
    {
        int yaw_total = 0;
        int pitch_total = 0; // Количество уровней без учёта вершин.

        linalg::vec<float, 3> *points_array =
            {}; // (size : yaw_total + pitch_total + 2)

    public:
        spherical_cloud_collector(int _yaw_total,
                                  int _pitch_total,
                                  linalg::vec<float, 3> *_points_array)
        {
            init(_yaw_total, _pitch_total, _points_array);
        }

        void init(int _yaw_total,
                  int _pitch_total,
                  linalg::vec<float, 3> *_points_array)
        {
            yaw_total = _yaw_total;
            pitch_total = _pitch_total;
            points_array = _points_array;
        }

        float yaw_by_index(int y)
        {
            float k = (float)y / (float)yaw_total; // обход без endpoint
            return 2 * M_PI * k;
        }

        float pitch_by_index(int p)
        {
            float k = (float)p / (float)(pitch_total - 1); // обход с endpoint
            return (-M_PI / 2) * (1.f - k) + (M_PI / 2) * k;
        }

        float pitch_koeff(float p)
        {
            return (p + M_PI / 2) / (M_PI);
        }

        float yaw_koeff(float y)
        {
            return (y) / (2 * M_PI);
        }

        linalg::vec<float, 2> angles_by_index(int idx)
        {
            if (idx == 0)
                return {0, -M_PI / 2};

            if (idx == yaw_total * (pitch_total - 2) + 1)
                return {0, M_PI / 2};

            int p = (idx - 1) / yaw_total + 1;
            int y = (idx - 1) % yaw_total;

            float r = yaw_by_index(y);
            float s = pitch_by_index(p);

            return {r, s};
        }

        int index_by_angles(linalg::vec<float, 2> vec)
        {
            assert(vec[0] >= 0 && vec[0] <= 2 * M_PI);
            assert(vec[1] >= float(-M_PI / 2) && vec[1] <= float(M_PI / 2));

            float y = vec[0];
            float p = vec[1];

            int yidx = yaw_koeff(y) * (yaw_total - 1) + 0.5;
            int pidx = pitch_koeff(p) * (pitch_total - 1) + 0.5;

            assert(yidx <= yaw_total && yidx >= 0);
            assert(pidx <= pitch_total && pidx >= 0);

            if (pidx == 0)
                return 0;
            if (pidx == pitch_total - 1)
                return (pitch_total - 2) * yaw_total + 1;

            return 1 + (pidx - 1) * yaw_total + yidx;
        }

        linalg::vec<float, 2> decart_to_spherical(linalg::vec<float, 3> pnt)
        {
            auto npnt = normalize(pnt);

            auto p = acosf(npnt[2]);
            auto y = atan2f(npnt[1], npnt[0]);

            return {y, p};
        }

        int bucket_for_decart_point(linalg::vec<float, 3> pnt)
        {
            return index_by_angles(decart_to_spherical(pnt));
        }

        void put_to_bucket(linalg::vec<float, 3> pnt)
        {
            int bucket = bucket_for_decart_point(pnt);
            points_array[bucket] = pnt;
        }

        linalg::vec<float, 3> center()
        {
            int total = 2 + (pitch_total - 2) * yaw_total;

            linalg::vec<float, 3> acc{0, 0, 0};
            for (int i = 0; i < total; ++i)
            {
                acc += points_array[i];
                if (points_array[i] == linalg::vec<float, 3>{0, 0, 0})
                    total--;
            }
            acc /= total;

            return acc;
        }
    };

    class magnetometer_calibrator
    {
    };
}

#endif
