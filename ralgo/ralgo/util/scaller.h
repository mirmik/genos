#ifndef RALGO_UTIL_SCALLER_H
#define RALGO_UTIL_SCALLER_H

// TODO: abstract inverse
// TODO: abstract reverse

namespace ralgo
{
    template <class K, class X = K> class scaller
    {
        K _gain = {};

    public:
        bool inited()
        {
            return _gain != 0;
        }

        scaller() = default;
        scaller(const K &koeff) : _gain(koeff) {}

        void set_gain(const K &koeff)
        {
            _gain = koeff;
        }
        K gain()
        {
            return _gain;
        }

        void set_gain(const K &koeff, bool reverse)
        {
            _gain = koeff * (reverse ? -1 : 1);
        }

        X ext2int(const X &x)
        {
            return _gain * x;
        }

        X int2ext(const X &x)
        {
            return x / _gain;
        }

        void swap_if_reverse(X &a, X &b)
        {
            if (_gain < 0)
                std::swap(a, b);
        }
    };
}

#endif
