#include "ralgo/imu/madgwick.h"
#include <igris/math/fast_invsqrt.h>

namespace ralgo
{
    madgwick::madgwick() {}

    void madgwick::set_koeffs(float _sampleFreq, float _beta)
    {
        beta = _beta;
        sampleFreq = _sampleFreq;
        invSampleFreq = (1.0f / _sampleFreq);
    }

    void madgwick::quat(float &x, float &y, float &z, float &w)
    {
        x = q1;
        y = q2;
        z = q3;
        w = q0;
    }

    void madgwick::reset()
    {
        q0 = 1;
        q1 = q2 = q3 = 0;
    }

    linalg::vec<float, 3> madgwick::earth2body(const linalg::vec<float, 3> &vec)
    {
        return linalg::qrot(qinv(quat()), vec);
    }

    linalg::vec<float, 3> madgwick::body2earth(const linalg::vec<float, 3> &vec)
    {
        return linalg::qrot(quat(), vec);
    }

    linalg::vec<float, 3> madgwick::gravity_direction()
    {
        return earth2body({0, 0, -1});
    }

    void madgwick::update(const linalg::vec<float, 3> &g,
                          const linalg::vec<float, 3> &a,
                          const linalg::vec<float, 3> &m)
    {
        update(g[0], g[1], g[2], a[0], a[1], a[2], m[0], m[1], m[2]);
    }

    void madgwick::update(const linalg::vec<float, 3> &g,
                          const linalg::vec<float, 3> &a)
    {
        update(g[0], g[1], g[2], a[0], a[1], a[2]);
    }

    void madgwick::update(const linalg::vec<float, 3> &g)
    {
        update(g[0], g[1], g[2]);
    }

    //---------------------------------------------------------------------------------------------------
    // AHRS algorithm update

    void madgwick::update(float gx,
                          float gy,
                          float gz,
                          float ax,
                          float ay,
                          float az,
                          float mx,
                          float my,
                          float mz)
    {
        float recipNorm;

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        apply(gx, gy, gz, ax, ay, az, mx, my, mz);
    }

    void madgwick::apply(float gx,
                         float gy,
                         float gz,
                         float ax,
                         float ay,
                         float az,
                         float mx,
                         float my,
                         float mz)
    {
        float module;
        float recipNorm;
        float s0, s1, s2, s3;
        float qDot1, qDot2, qDot3, qDot4;
        // float hx, hy;
        float _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3,
            q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

        // Rate of change of quaternion from gyroscope
        qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
        qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
        qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
        qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

        // Auxiliary variables to avoid repeated arithmetic
        //        _2q0mx = 2.0f * q0 * mx;
        //        _2q0my = 2.0f * q0 * my;
        //        _2q0mz = 2.0f * q0 * mz;
        //        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        // q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth's magnetic field
        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) +
                     mz * (q1q3 + q0q2)); // проверено
        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) +
                     mz * (q2q3 - q0q1)); // проверено
        // bx = sqrt(hx * hx + hy * hy);
        // bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f -
        // q1q1 - q2q2));

        _2bx = sqrtf(hx * hx + hy * hy);
        //_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz *
        // q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
        _2bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) +
                       mz * (0.5f - q1q1 - q2q2));
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) +
             _2q1 * (2.0f * q0q1 + _2q2q3 - ay) -
             _2bz * q2 *
                 (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) +
             (-_2bx * q3 + _2bz * q1) *
                 (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) +
             _2bx * q2 *
                 (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) +
             _2q0 * (2.0f * q0q1 + _2q2q3 - ay) -
             4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) +
             _2bz * q3 *
                 (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) +
             (_2bx * q2 + _2bz * q0) *
                 (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) +
             (_2bx * q3 - _4bz * q1) *
                 (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) +
             _2q3 * (2.0f * q0q1 + _2q2q3 - ay) -
             4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) +
             (-_4bx * q2 - _2bz * q0) *
                 (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) +
             (_2bx * q1 + _2bz * q3) *
                 (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) +
             (_2bx * q0 - _4bz * q2) *
                 (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) +
             _2q2 * (2.0f * q0q1 + _2q2q3 - ay) +
             (-_4bx * q3 + _2bz * q1) *
                 (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) +
             (-_2bx * q0 + _2bz * q2) *
                 (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) +
             _2bx * q1 *
                 (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);

        module = s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3;
        if (module > 1)
        {
            recipNorm = invSqrt(module);
            s0 *= recipNorm;
            s1 *= recipNorm;
            s2 *= recipNorm;
            s3 *= recipNorm;
        }

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;

        // Integrate rate of change of quaternion to yield quaternion
        q0 += qDot1 * invSampleFreq;
        q1 += qDot2 * invSampleFreq;
        q2 += qDot3 * invSampleFreq;
        q3 += qDot4 * invSampleFreq;

        // Normalise quaternion
        recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
        q0 *= recipNorm;
        q1 *= recipNorm;
        q2 *= recipNorm;
        q3 *= recipNorm;
    }

    //---------------------------------------------------------------------------------------------------
    // IMU algorithm update

    void
    madgwick::update(float gx, float gy, float gz, float ax, float ay, float az)
    {
        float recipNorm;

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        apply(gx, gy, gz, ax, ay, az);
    }

    void
    madgwick::apply(float gx, float gy, float gz, float ax, float ay, float az)
    {
        float module;
        float recipNorm;
        float s0, s1, s2, s3;
        float qDot1, qDot2, qDot3, qDot4;
        float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1,
            q2q2, q3q3;

        // Rate of change of quaternion from gyroscope
        qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
        qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
        qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
        qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _4q0 = 4.0f * q0;
        _4q1 = 4.0f * q1;
        _4q2 = 4.0f * q2;
        _8q1 = 8.0f * q1;
        _8q2 = 8.0f * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;

        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 +
             _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 +
             _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;

        module = s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3;
        if (module > 1)
        {
            recipNorm = invSqrt(module);
            s0 *= recipNorm;
            s1 *= recipNorm;
            s2 *= recipNorm;
            s3 *= recipNorm;
        }

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;

        // Integrate rate of change of quaternion to yield quaternion
        q0 += qDot1 * (1.0f / sampleFreq);
        q1 += qDot2 * (1.0f / sampleFreq);
        q2 += qDot3 * (1.0f / sampleFreq);
        q3 += qDot4 * (1.0f / sampleFreq);

        // Normalise quaternion
        recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
        q0 *= recipNorm;
        q1 *= recipNorm;
        q2 *= recipNorm;
        q3 *= recipNorm;
    }

    void madgwick::update(float gx, float gy, float gz)
    {
        float recipNorm;
        float qDot1, qDot2, qDot3, qDot4;

        // Rate of change of quaternion from gyroscope
        qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
        qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
        qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
        qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

        // Integrate rate of change of quaternion to yield quaternion
        q0 += qDot1 * invSampleFreq;
        q1 += qDot2 * invSampleFreq;
        q2 += qDot3 * invSampleFreq;
        q3 += qDot4 * invSampleFreq;

        // Normalise quaternion
        recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
        q0 *= recipNorm;
        q1 *= recipNorm;
        q2 *= recipNorm;
        q3 *= recipNorm;
    }

    //---------------------------------------------------------------------------------------------------
    // Fast inverse square-root
    // See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

    float madgwick::invSqrt(float x)
    {
        // if (x==0) return 0;
        return 1 / sqrtf(x); // quick_invsqrt(x);
    }

    float madgwick::getYawRad()
    {
        return atan2(2 * q1 * q2 - 2 * q0 * q3, 2 * q0 * q0 + 2 * q1 * q1 - 1);
    }

    float madgwick::getPitchRad()
    {
        return atan2(2 * q2 * q3 - 2 * q0 * q1, 2 * q0 * q0 + 2 * q3 * q3 - 1);
    }

    float madgwick::getRollRad()
    {
        return -1 * atan2(2.0f * (q0 * q2 - q1 * q3),
                          1.0f - 2.0f * (q2 * q2 + q1 * q1));
    }

    float madgwick::getYawDeg()
    {
        return getYawRad() * RAD_TO_DEG;
    }

    float madgwick::getPitchDeg()
    {
        return getPitchRad() * RAD_TO_DEG;
    }

    float madgwick::getRollDeg()
    {
        return getRollRad() * RAD_TO_DEG;
    }

    void madgwick::ZYX(float &z, float &y, float &x)
    {
        z = atan2(q1 * q2 + q3 * q0, 0.5 - q2 * q2 - q3 * q3);
        y = -asin(2 * (q1 * q3 - q2 * q0));
        x = atan2(q2 * q3 + q1 * q0, 0.5 - q1 * q1 - q2 * q2);
    }

    linalg::vec<float, 3> madgwick::ZYX()
    {
        linalg::vec<float, 3> ret;
        ZYX(ret[0], ret[1], ret[2]);
        return ret;
    }

    void madgwick::ZYZ(float &z, float &y, float &z2)
    {
        z = atan2(q2 * q3 - q1 * q0, q1 * q3 + q2 * q0);
        y = acos(1 - 2 * (q1 * q1 + q2 * q2));
        z2 = -atan2(q2 * q3 + q1 * q0, q1 * q3 - q2 * q0);
    }

    void madgwick::ZYZ_u(float &z, float &y, float &z2)
    {
        z = atan2(q1 * q2 + q3 * q0, 0.5 - (q2 * q2 + q3 * q3));
        y = acos(1 - 2 * (q1 * q1 + q2 * q2));
        z2 = -atan2(q2 * q3 + q1 * q0, q1 * q3 - q2 * q0);
    }

    linalg::vec<float, 3>
    madgwick::horizon_projection(const linalg::vec<float, 3> &vec)
    {
        auto gravity = gravity_direction();

        auto zaxis_projection_norm = linalg::dot(gravity, vec);
        auto zaxis_projection = gravity * zaxis_projection_norm;

        return vec - zaxis_projection;
    }
}
