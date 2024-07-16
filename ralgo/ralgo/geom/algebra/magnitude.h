#ifndef RALGO_GEOMALG_MAGNITUDE_H
#define RALGO_GEOMALG_MAGNITUDE_H

namespace ralgo
{
    namespace geomalg
    {
        template <class T> class magnitude
        {
            T _s;
            T _w;

        public:
            magnitude(T s, T w) : _s(s), _w(w) {}

            T s()
            {
                return _s;
            }

            T w()
            {
                return _w;
            }

            magnitude unitized()
            {
                return {_s / _w, 1};
            }
        };
    }
}

#endif