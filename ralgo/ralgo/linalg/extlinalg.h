#ifndef RALGO_EXTLINALG_H
#define RALGO_EXTLINALG_H

#include <ralgo/linalg/linalg.h>
#include <ralgo/space/pose3.h>

namespace ralgo
{
    template <class T>
    linalg::mat<T, 3, 3> antisymmetric_tensor(const linalg::vec<T, 3> &v)
    {
        return {{0, v[2], -v[1]}, {-v[2], 0, v[0]}, {v[1], -v[0], 0}};
    }

    template <class T> linalg::mat<T, 3, 3> diag(const linalg::vec<T, 3> &v)
    {
        return {{v[0], 0, 0}, {0, v[1], 0}, {0, 0, v[2]}};
    }

    template <class T>
    linalg::mat<T, 3, 3> outer(const linalg::vec<T, 3> &v1,
                               const linalg::vec<T, 3> &v2)
    {
        return {{v1[0] * v2[0], v1[1] * v2[0], v1[2] * v2[0]},
                {v1[0] * v2[1], v1[1] * v2[1], v1[2] * v2[1]},
                {v1[0] * v2[2], v1[1] * v2[2], v1[2] * v2[2]}};
    }

    /// @brief Rotate a quadric form by quaternion
    template <class T>
    linalg::mat<T, 3, 3> rotate_quadric_form(const linalg::vec<T, 4> &q,
                                             const linalg::mat<T, 3, 3> &in)
    {
        linalg::mat<T, 3, 3> rotmat = {qxdir(q), qydir(q), qzdir(q)};
        auto out0 = linalg::mul<T>(rotmat, in);
        auto out1 = linalg::mul<T>(out0, linalg::transpose(rotmat));
        return out1;
    }

    /// @brief Rotate a quadric form by quaternion
    template <class T>
    linalg::mat<T, 3, 3> rotate_quadric_form(const linalg::vec<T, 4> &q,
                                             const linalg::vec<T, 3> &in)
    {
        return rotate_quadric_form(q, ralgo::diag<T>(in));
    }

    /// \brief Теорема Штайнера. Вычисляет тензор момента инерции по
    /// \param
    template <class T>
    linalg::mat<T, 3, 3> steiner(T mass,
                                 const linalg::vec<T, 3> &local_inertia,
                                 const linalg::vec<T, 3> &local_center,
                                 const ralgo::pose3<T> &world_pose,
                                 const linalg::vec<T, 3> &target_center)
    {
        auto wf_center = world_pose.transform_point(local_center);
        auto wf_inertia = rotate_quadric_form(world_pose.ang, local_inertia);
        auto radius = target_center - wf_center;
        auto scalar_square = radius[0] * radius[0] + radius[1] * radius[1] +
                             radius[2] * radius[2];
        auto kroneker_prod =
            ralgo::diag<T>({scalar_square, scalar_square, scalar_square});
        auto outer_prod = ralgo::outer(radius, radius);
        auto shteiner = wf_inertia + mass * (kroneker_prod - outer_prod);
        return shteiner;
    }
}

#endif