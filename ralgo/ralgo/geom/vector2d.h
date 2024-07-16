#ifndef RALGO_GEOMALG_VECTOR2D_H
#define RALGO_GEOMALG_VECTOR2D_H

#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    namespace geom
    {
        template <class T> class vector2d
        {
            T _x;
            T _y;

        public:
            vector2d() = default;
            vector2d(T x, T y) : _x(x), _y(y) {}
            vector2d(const vector2d &v) = default;
            vector2d &operator=(const vector2d &v) = default;

            T x() const
            {
                return _x;
            }

            T y() const
            {
                return _y;
            }
        };

        template <class T> class point2d
        {
            T _x;
            T _y;

        public:
            point2d() = default;
            point2d(T x, T y) : _x(x), _y(y) {}
            point2d(const point2d &v) = default;
            point2d &operator=(const point2d &v) = default;

            T x() const
            {
                return _x;
            }

            T y() const
            {
                return _y;
            }
        };

        template <class T>
        vector2d<T> operator-(const point2d<T> &a, const point2d<T> &b)
        {
            return vector2d<T>(a.x() - b.x(), a.y() - b.y());
        }

        template <class T>
        point2d<T> operator+(const point2d<T> &a, const vector2d<T> &b)
        {
            return point2d<T>(a.x() + b.x(), a.y() + b.y());
        }

        template <class T>
        point2d<T> operator-(const point2d<T> &a, const vector2d<T> &b)
        {
            return point2d<T>(a.x() - b.x(), a.y() - b.y());
        }

        template <class T>
        vector2d<T> operator+(const vector2d<T> &a, const vector2d<T> &b)
        {
            return vector2d<T>(a.x() + b.x(), a.y() + b.y());
        }

        template <class T>
        vector2d<T> operator-(const vector2d<T> &a, const vector2d<T> &b)
        {
            return vector2d<T>(a.x() - b.x(), a.y() - b.y());
        }

        template <class T> vector2d<T> operator*(const vector2d<T> &a, T b)
        {
            return vector2d<T>(a.x() * b, a.y() * b);
        }

        template <class T> vector2d<T> operator*(T a, const vector2d<T> &b)
        {
            return vector2d<T>(a * b.x(), a * b.y());
        }
    }
}

#endif