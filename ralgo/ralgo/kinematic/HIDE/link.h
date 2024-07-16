#ifndef CYNEMATIC_LINK_H
#define CYNEMATIC_LINK_H

/**
    @file
*/

//#include <linalg-add.h>
#include <linalg-v3/linalg-ext.h>
#include <linalg-v3/linalg.h>

//#include <nos/trace.h>
//#include <gxx/panic.h>

#include <algorithm>
#include <initializer_list>

//#include <gxx/util/iteration_counter.h>

#include <chrono>
#include <thread>

#include <igris/util/bug.h>
#include <numeric>

#include <nos/input.h>

#include <malgo/vector.h>
#include <ralgo/linalg/bivec.h>

//#define EPSILON 0.000001
//#define EPSILON2 0.0000001

namespace ralgo
{
    namespace cynematic
    {
        using namespace linalg;
        using namespace linalg::aliases;
        using namespace linalg::ostream_overloads;

        template <typename T> struct abstract_link
        {
            using ax_t = linalg::vec<T, 3>;

            virtual htrans<T> get(const malgo::vector<T> &coords,
                                  uint8_t pos) = 0;

            virtual uint8_t count_of_coords() = 0;
        };

        template <typename T> struct onedof_link : public abstract_link<T>
        {
            virtual htrans<T> get(T coord) = 0;

            htrans<T> get(const malgo::vector<T> &coords, uint8_t pos) override
            {
                return get(coords[pos]);
            }

            virtual bivec<T, 3> d1_bivec() = 0;
        };

        template <typename T> struct constant_link : public abstract_link<T>
        {
            htrans<T> mat;

            htrans<T> get()
            {
                return mat;
            }

            htrans<T> get(const malgo::vector<T> &coords, uint8_t pos) override
            {
                return mat;
            }

            uint8_t count_of_coords() override
            {
                return 0;
            }

            constant_link(htrans<T> _mat) : mat(_mat){};
        };

        template <typename T> struct rotation_link : public onedof_link<T>
        {
            using ax_t = linalg::vec<T, 3>;
            ax_t axvec;

            rotation_link(ax_t _axvec) : axvec(_axvec) {}

            htrans<T> get(T coord) override
            {
                return htrans<T>::rotation(axvec, coord);
            }

            uint8_t count_of_coords() override
            {
                return 1;
            }

            bivec<T, 3> d1_bivec() override
            {
                return {axvec, ax_t()};
            }
        };

        template <typename T> struct translation_link : public onedof_link<T>
        {
            using ax_t = linalg::vec<T, 3>;
            ax_t axvec;

            translation_link(ax_t _axvec) : axvec(_axvec) {}

            htrans<T> get(T coord) override
            {
                return htrans<T>::translation(axvec * coord);
            }

            uint8_t count_of_coords() override
            {
                return 1;
            }

            bivec<T, 3> d1_bivec() override
            {
                return {ax_t(), axvec};
            }
        };
    }
}
#endif
