#ifndef RALGO_KINEMATIC_LINK2D_H
#define RALGO_KINEMATIC_LINK2D_H

#include <ralgo/space/htrans2.h>
//#include <ralgo/planning/phase_driver.h>

namespace ralgo
{
    class unit2d
    {
    public:
        unit2d *parent = nullptr;

        ralgo::htrans2<float> local_location;
        ralgo::htrans2<float> global_location;

    public:
        unit2d() : local_location(), global_location() {}

        void update_location()
        {
            if (parent)
                global_location = parent->global_location * local_location;
            else
                global_location = local_location;
        }

        void relocate(const ralgo::htrans2<float> &trans)
        {
            local_location = trans;
        }

        virtual bool iscynem()
        {
            return false;
        }
    };

    class kinematic_unit2d : public unit2d
    {
    public:
        unit2d output;
        double coord = 0;

    public:
        kinematic_unit2d()
        {
            output.parent = this;
        }

        void link(unit2d *oth)
        {
            oth->parent = &output;
        }

        virtual ralgo::screw2<float> sensivity() = 0;

        bool iscynem() override
        {
            return true;
        }
    };

    class unit2d_1dof : public kinematic_unit2d
    {
    public:
        virtual void set_coord(float coord) = 0;
    };

    class rotator2 : public unit2d_1dof
    {
        float mul;

    public:
        rotator2() : mul(1) {}
        rotator2(float mul) : mul(mul) {}

        void set_coord(float angle)
        {
            coord = angle;
            output.local_location = ralgo::htrans2<float>(angle * mul, {0, 0});
        }

        ralgo::screw2<float> sensivity() override
        {
            return {mul, {0, 0}};
        }
    };

    class actuator2 : public unit2d_1dof
    {
        linalg::vec<float, 2> ax;
        float mul;

    public:
        actuator2(linalg::vec<float, 2> ax, float mul) : ax(ax), mul(mul) {}

        void set_coord(float c)
        {
            coord = c;
            output.local_location = ralgo::htrans2<float>(0, ax * mul * c);
        }

        ralgo::screw2<float> sensivity() override
        {
            return {0, mul * ax};
        }
    };
}

#endif
