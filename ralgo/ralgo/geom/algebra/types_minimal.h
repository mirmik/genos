#ifndef RALGO_GEOMALG_TYPES_MINIMAL_H
#define RALGO_GEOMALG_TYPES_MINIMAL_H

template <bool Enable, class T> class test_enablable
{
public:
    T value;
    test_enablable &operator=(const T &v)
    {
        value = v;
        return *this;
    }

    operator T() const
    {
        return value;
    }
};

template <class T> class test_enablable<false, T>
{
public:
    operator T() const
    {
        return 0;
    }
};

template <class T, bool _E1, bool _E2, bool _E3> class test_multivector
{
public:
    [[no_unique_address]] test_enablable<_E1, T> e1 = {};
    [[no_unique_address]] test_enablable<_E2, T> e2 = {};
    [[no_unique_address]] test_enablable<_E3, T> e3 = {};
    test_multivector() {}
};

static_assert(sizeof(test_multivector<double, false, true, true>) ==
              2 * sizeof(double)); // passed
static_assert(sizeof(test_multivector<double, true, true, false>) ==
              2 * sizeof(double)); // passed
static_assert(sizeof(test_multivector<double, false, false, false>) ==
              3); // passed
static_assert(sizeof(test_multivector<double, false, false, true>) ==
              1 * sizeof(double)); // passed
static_assert(sizeof(test_multivector<double, true, false, true>) ==
              2 * sizeof(double)); // passed
/*static_assert(sizeof(test_multivector<double, false, true, false>) ==
              1 * sizeof(double)); // failed*/

#endif