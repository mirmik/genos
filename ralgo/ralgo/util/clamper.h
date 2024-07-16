#ifndef RALGO_UTIL_CLAMPER_H
#define RALGO_UTIL_CLAMPER_H

namespace ralgo
{
    template <class T> class clamper
    {
    public:
        T lo = T();
        T hi = T();
        bool enabled = true;
        bool _inited = false;

    public:
        clamper(bool en = true) : enabled(en) {}

        bool inited()
        {
            return _inited;
        }

        void set_limits(const T &lo, const T &hi)
        {
            _inited = true;
            this->lo = lo;
            this->hi = hi;
        }

        void enable(bool en)
        {
            enabled = en;
        }

        T operator()(const T &x)
        {
            if (!enabled)
                return x;

            return std::clamp(x, lo, hi);
        }

        T operator()(const T &x, bool reversed)
        {
            if (!enabled)
                return x;

            if (reversed)
                return std::clamp(x, -hi, -lo);
            else
                return std::clamp(x, lo, hi);
        }
    };
}

#endif
