#ifndef RALGO_GEOMALG_TYPES_H
#define RALGO_GEOMALG_TYPES_H

#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    namespace geomalg
    {
        template <class T, bool Enable = true> class enablable
        {
        public:
            T value;
            enablable &operator=(const T &v)
            {
                value = v;
                return *this;
            }

            operator T() const
            {
                return value;
            }
        };

        template <class T> class enablable<T, false>
        {
        public:
            operator T() const
            {
                return 0;
            }
        };

        template <class T1, class T2, bool En1, bool En2>
        auto operator+(const enablable<T1, En1> &a, const enablable<T2, En2> &b)
        {
            if constexpr (En1 && En2)
                return enablable<decltype(a.value + b.value), true>{a.value +
                                                                    b.value};
            else if constexpr (En1)
                return enablable<T1, true>{a.value};
            else if constexpr (En2)
                return enablable<T2, true>{b.value};
            else
                return enablable<T1, false>{};
        }

        template <class T1, class T2, bool En1, bool En2>
        auto operator*(const enablable<T1, En1> &a, const enablable<T2, En2> &b)
        {
            if constexpr (En1 && En2)
                return enablable<decltype(a.value * b.value), true>{a.value *
                                                                    b.value};
            else if constexpr (En1)
                return enablable<T1, false>{};
            else if constexpr (En2)
                return enablable<T2, false>{};
            else
                return enablable<T1, false>{};
        }

        template <class T,
                  bool _E = false,
                  bool _E1 = false,
                  bool _E2 = false,
                  bool _E3 = false,
                  bool _E4 = false,
                  bool _E23 = false,
                  bool _E31 = false,
                  bool _E12 = false,
                  bool _E43 = false,
                  bool _E42 = false,
                  bool _E41 = false,
                  bool _E321 = false,
                  bool _E412 = false,
                  bool _E431 = false,
                  bool _E423 = false,
                  bool _E4321 = false>
        class multivector
        {
        public:
            enablable<T, _E> e = {};
            enablable<T, _E1> e1 = {};
            enablable<T, _E2> e2 = {};
            enablable<T, _E3> e3 = {};
            enablable<T, _E4> e4 = {};
            enablable<T, _E23> e23 = {};
            enablable<T, _E31> e31 = {};
            enablable<T, _E12> e12 = {};
            enablable<T, _E43> e43 = {};
            enablable<T, _E42> e42 = {};
            enablable<T, _E41> e41 = {};
            enablable<T, _E321> e321 = {};
            enablable<T, _E412> e412 = {};
            enablable<T, _E431> e431 = {};
            enablable<T, _E423> e423 = {};
            enablable<T, _E4321> e4321 = {};

            multivector() {}
        };

        template <class T_1,
                  bool _E_1,
                  bool _E1_1,
                  bool _E2_1,
                  bool _E3_1,
                  bool _E4_1,
                  bool _E23_1,
                  bool _E31_1,
                  bool _E12_1,
                  bool _E43_1,
                  bool _E42_1,
                  bool _E41_1,
                  bool _E321_1,
                  bool _E412_1,
                  bool _E431_1,
                  bool _E423_1,
                  bool _E4321_1,
                  class T_2,
                  bool _E_2,
                  bool _E1_2,
                  bool _E2_2,
                  bool _E3_2,
                  bool _E4_2,
                  bool _E23_2,
                  bool _E31_2,
                  bool _E12_2,
                  bool _E43_2,
                  bool _E42_2,
                  bool _E41_2,
                  bool _E321_2,
                  bool _E412_2,
                  bool _E431_2,
                  bool _E423_2,
                  bool _E4321_2>
        auto operator+(const multivector<T_1,
                                         _E_1,
                                         _E1_1,
                                         _E2_1,
                                         _E3_1,
                                         _E4_1,
                                         _E23_1,
                                         _E31_1,
                                         _E12_1,
                                         _E43_1,
                                         _E42_1,
                                         _E41_1,
                                         _E321_1,
                                         _E412_1,
                                         _E431_1,
                                         _E423_1,
                                         _E4321_1> &a,
                       const multivector<T_2,
                                         _E_2,
                                         _E1_2,
                                         _E2_2,
                                         _E3_2,
                                         _E4_2,
                                         _E23_2,
                                         _E31_2,
                                         _E12_2,
                                         _E43_2,
                                         _E42_2,
                                         _E41_2,
                                         _E321_2,
                                         _E412_2,
                                         _E431_2,
                                         _E423_2,
                                         _E4321_2> &b)
        {
            multivector<decltype(std::declval<T_1>() + std::declval<T_2>()),
                        _E_1 || _E_2,
                        _E1_1 || _E1_2,
                        _E2_1 || _E2_2,
                        _E3_1 || _E3_2,
                        _E4_1 || _E4_2,
                        _E23_1 || _E23_2,
                        _E31_1 || _E31_2,
                        _E12_1 || _E12_2,
                        _E43_1 || _E43_2,
                        _E42_1 || _E42_2,
                        _E41_1 || _E41_2,
                        _E321_1 || _E321_2,
                        _E412_1 || _E412_2,
                        _E431_1 || _E431_2,
                        _E423_1 || _E423_2,
                        _E4321_1 || _E4321_2>
                result;
            result.e = a.e + b.e;
            result.e1 = a.e1 + b.e1;
            result.e2 = a.e2 + b.e2;
            result.e3 = a.e3 + b.e3;
            result.e4 = a.e4 + b.e4;
            result.e23 = a.e23 + b.e23;
            result.e31 = a.e31 + b.e31;
            result.e12 = a.e12 + b.e12;
            result.e43 = a.e43 + b.e43;
            result.e42 = a.e42 + b.e42;
            result.e41 = a.e41 + b.e41;
            result.e321 = a.e321 + b.e321;
            result.e412 = a.e412 + b.e412;
            result.e431 = a.e431 + b.e431;
            result.e423 = a.e423 + b.e423;
            result.e4321 = a.e4321 + b.e4321;
            return result;
        }

        template <class T_1,
                  bool A,
                  bool A1,
                  bool A2,
                  bool A3,
                  bool A4,
                  bool A23,
                  bool A31,
                  bool A12,
                  bool A43,
                  bool A42,
                  bool A41,
                  bool A321,
                  bool A412,
                  bool A431,
                  bool A423,
                  bool A4321,
                  class T_2,
                  bool B,
                  bool B1,
                  bool B2,
                  bool B3,
                  bool B4,
                  bool B23,
                  bool B31,
                  bool B12,
                  bool B43,
                  bool B42,
                  bool B41,
                  bool B321,
                  bool B412,
                  bool B431,
                  bool B423,
                  bool B4321>
        auto prod(const multivector<T_1,
                                    A,
                                    A1,
                                    A2,
                                    A3,
                                    A4,
                                    A23,
                                    A31,
                                    A12,
                                    A43,
                                    A42,
                                    A41,
                                    A321,
                                    A412,
                                    A431,
                                    A423,
                                    A4321> &a,
                  const multivector<T_2,
                                    B,
                                    B1,
                                    B2,
                                    B3,
                                    B4,
                                    B23,
                                    B31,
                                    B12,
                                    B43,
                                    B42,
                                    B41,
                                    B321,
                                    B412,
                                    B431,
                                    B423,
                                    B4321> &b)
        {
            multivector<
                decltype(std::declval<T_1>() + std::declval<T_2>()),
                A && B || A1 && B1 || A2 && B2 || A3 && B3 || A23 && B23 ||
                    A31 && B31 || A12 && B12 || A321 && B321,
                A && B1 || A1 && B || A3 && B31 || A31 && B3 || A2 && B12 ||
                    A12 && B2 || A23 && B321 || A321 && B23,
                A && B2 || A2 && B || A3 && B23 || A23 && B3 || A1 && B12 ||
                    A12 && B1 || A31 && B321 || A321 && B31,
                A && B3 || A3 && B || A1 && B31 || A31 && B1 || A2 && B23 ||
                    A23 && B2 || A12 && B321 || A321 && B12,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false>
                result;
            result.e = a.e * b.e + a.e1 * b.e1 + a.e2 * b.e2 + a.e3 * b.e3 -
                       a.e23 * b.e23 - a.e31 * b.e31 - a.e12 * b.e12 -
                       a.e321 * b.e321;
            result.e1 = a.e * b.e1 + a.e1 * b.e + a.e3 * b.e31 - a.e31 * b.e3 +
                        -a.e2 * b.e12 + a.e12 * b.e2 + a.e23 * b.e321 +
                        a.e321 * b.e23;
            result.e2 = a.e * b.e2 + a.e2 * b.e - a.e3 * b.e23 + a.e23 * b.e3 +
                        a.e1 * b.e12 - a.e12 * b.e1 + a.e31 * b.e321 +
                        a.e321 * b.e31;
            result.e3 = a.e * b.e3 + a.e3 * b.e - a.e1 * b.e31 + a.e31 * b.e1 +
                        a.e2 * b.e23 - a.e23 * b.e2 + a.e12 * b.e321 +
                        a.e321 * b.e12;
            /*
            result.e4 = a.e4 + b.e4;
            result.e23 = a.e23 + b.e23;
            result.e31 = a.e31 + b.e31;
            result.e12 = a.e12 + b.e12;
            result.e43 = a.e43 + b.e43;
            result.e42 = a.e42 + b.e42;
            result.e41 = a.e41 + b.e41;
            result.e321 = a.e321 + b.e321;
            result.e412 = a.e412 + b.e412;
            result.e431 = a.e431 + b.e431;
            result.e423 = a.e423 + b.e423;
            result.e4321 = a.e4321 + b.e4321;*/
            return result;
        }

        template <class T>
        class vector : public multivector<T,
                                          false,
                                          true,
                                          true,
                                          true,
                                          false,
                                          false,
                                          false,
                                          false,
                                          false,
                                          false,
                                          false,
                                          false,
                                          false,
                                          false,
                                          false,
                                          false>
        {
            using base = multivector<T,
                                     false,
                                     true,
                                     true,
                                     true,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false>;

        public:
            vector() {}
            vector(double x, double y, double z)
            {
                base::e1 = x;
                base::e2 = y;
                base::e3 = z;
            }
        };

        template <class T>
        class bivector : public multivector<T,
                                            false,
                                            false,
                                            false,
                                            false,
                                            false,
                                            true,
                                            true,
                                            true,
                                            false,
                                            false,
                                            false,
                                            false,
                                            false,
                                            false,
                                            false,
                                            false>
        {
            using base = multivector<T,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     true,
                                     true,
                                     true,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false,
                                     false>;

        public:
            bivector() {}
            bivector(double x, double y, double z)
            {
                base::e23 = x;
                base::e31 = y;
                base::e12 = z;
            }
        };

    }
}

#endif