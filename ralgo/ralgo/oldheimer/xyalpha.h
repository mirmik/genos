#ifndef HEIMER_XYALPHA_COORDINATE_CONTROLLER_H
#define HEIMER_XYALPHA_COORDINATE_CONTROLLER_H

/*
    Данный контроллер реализует управление
    кинематической цепью из двух ортагональных актуаторов и
    поворотного звена, перенося управляющие оси в точку выходного
    звена

    Аппаратные оси:
                       ^ y
             alpha     |
          x----O-----| |
                     | |
                     |
                     |
                     |
                    ---
        ================= ---> x

    Виртуальные оси:

          ^ y
          |
          |
    alpha x----O-----|
          ---->      |
              x      |
                     |
                     |
                    ---
        =================
*/

#include <nos/fprint.h>
#include <nos/print.h>

#include <ralgo/disctime.h>

#include <ralgo/oldheimer/kin2d_controller.h>
#include <ralgo/space/htrans2.h>
#include <ralgo/space/screw.h>

namespace heimer
{
    template <class P, class V>
    class xyalpha_chain2d_controller : public kinematic_chain2d_controller<P, V>
    {
        using kin2d = kinematic_chain2d_controller<P, V>;
        using kin2d::chain;

        ralgo::unit2d *axylinks[7];
        ralgo::kinematic_unit2d *axypairs[3];

        double ctrpos[3];
        double ctrspd[3];

        int64_t lasttime = 0;

    public:
        ralgo::actuator2 x_link;
        ralgo::actuator2 y_link;
        ralgo::rotator2 a_link;
        ralgo::unit2d output_link;

        heimer::axis_node<P, V> *x_controlled;
        heimer::axis_node<P, V> *y_controlled;
        heimer::axis_node<P, V> *a_controlled;

        ralgo::htrans2<float> nullpos;
        ralgo::htrans2<float> invnullpos;

        ralgo::htrans2<float> outpos;

        union
        {
            virtual_axis_node<P, V> ctraxes[3];

            struct
            {
                virtual_axis_node<P, V> x_axis;
                virtual_axis_node<P, V> y_axis;
                virtual_axis_node<P, V> a_axis;
            };
        };

        P axposes[3];

    public:
        xyalpha_chain2d_controller(const char *name,
                                   const char *xname,
                                   const char *yname,
                                   const char *aname,
                                   axis_node<P, V> *x_controlled,
                                   axis_node<P, V> *y_controlled,
                                   axis_node<P, V> *a_controlled)
            : kin2d(name),

              x_link({1, 0}, 1), y_link({0, 1}, 1), a_link(1),

              x_axis(xname, this), y_axis(yname, this), a_axis(aname, this)
        {
            x_link.link(&y_link);
            y_link.link(&a_link);
            a_link.link(&output_link);

            kin2d::setup(axylinks, axypairs, &output_link);

            this->x_controlled = x_controlled;
            this->y_controlled = y_controlled;
            this->a_controlled = a_controlled;
        }

        void relocate(ralgo::htrans2<float> x,
                      ralgo::htrans2<float> y,
                      ralgo::htrans2<float> a,
                      ralgo::htrans2<float> out)
        {
            x_link.relocate(x);
            y_link.relocate(y);
            a_link.relocate(a);
            output_link.relocate(out);
        }

        // Расчитывает положение и винт скорости
        // для текущей управляющей конфигурации.
        void get_control_phase(int64_t time,
                               ralgo::htrans2<float> &pos,
                               ralgo::screw2<float> &spd)
        {
            P xpos, ypos, apos;
            V xspd, yspd, aspd;

            xpos = x_axis.ctrpos;
            ypos = y_axis.ctrpos;
            apos = a_axis.ctrpos;

            yspd = y_axis.ctrspd;
            xspd = x_axis.ctrspd;
            aspd = a_axis.ctrspd;

            pos = ralgo::htrans2<float>{apos, {xpos, ypos}};
            spd = ralgo::screw2<float>{aspd, {xspd, yspd}};

            pos = nullpos * pos;
            spd = nullpos.rotate_screw(spd);
        }

        void restore_control_model() override
        {
            double xpos, ypos, apos;

            xpos = x_controlled->feedback_position();
            ypos = y_controlled->feedback_position();
            apos = a_controlled->feedback_position();

            x_link.set_coord(xpos);
            y_link.set_coord(ypos);
            a_link.set_coord(apos);

            chain.update_location();
            outpos = chain.out()->global_location;

            auto outpos_corrected = invnullpos * outpos;

            x_axis.restore_control(outpos_corrected.translation().x, 0);
            y_axis.restore_control(outpos_corrected.translation().y, 0);
            a_axis.restore_control(outpos_corrected.rotation(), 0);
        }

        void feedback()
        {
            double xpos = x_controlled->feedback_position();
            double ypos = y_controlled->feedback_position();
            double apos = a_controlled->feedback_position();

            x_link.set_coord(xpos);
            y_link.set_coord(ypos);
            a_link.set_coord(apos);

            chain.update_location();
            outpos = chain.out()->global_location;

            auto outpos_corrected = invnullpos * outpos;

            x_axis.feedpos = outpos_corrected.translation().x;
            y_axis.feedpos = outpos_corrected.translation().y;
            a_axis.feedpos = outpos_corrected.rotation();

            // TODO: Реимплементировать через подчиненные оси
            x_axis.feedspd = x_axis.ctrspd;
            y_axis.feedspd = y_axis.ctrspd;
            a_axis.feedspd = a_axis.ctrspd;
        }

        double *ctrspd_array() override
        {
            return ctrspd;
        }

        void apply_control()
        {
            int64_t time = ralgo::discrete_time();
            lasttime = time;

            x_controlled->control(x_controlled->feedback_position(), ctrspd[0]);
            y_controlled->control(y_controlled->feedback_position(), ctrspd[1]);
            a_controlled->control(a_controlled->feedback_position(), ctrspd[2]);
        }

        void print_info() override
        {
            nos::println("not implemented");
        }

        control_node *iterate(control_node *slt) override
        {
            if (slt == nullptr)
                return x_controlled;

            if (slt == x_controlled)
                return y_controlled;

            if (slt == y_controlled)
                return a_controlled;

            if (slt == a_controlled)
                return nullptr;
            else
                return nullptr;
        }

        int on_activate() override
        {
            x_axis.flags |= HEIM_IS_ACTIVE;
            y_axis.flags |= HEIM_IS_ACTIVE;
            a_axis.flags |= HEIM_IS_ACTIVE;
            return 0;
        }

        int on_deactivate() override
        {
            x_axis.flags &= ~HEIM_IS_ACTIVE;
            y_axis.flags &= ~HEIM_IS_ACTIVE;
            a_axis.flags &= ~HEIM_IS_ACTIVE;
            return 0;
        }

        virtual bool on_interrupt(control_node *slave,
                                  control_node *source,
                                  interrupt_args *data)
        {
            if (data->code() == HEIMER_INTERRUPT_TYPE_CONTROL_UPDATE)
            {
                restore_control_model();
            }

            x_axis.rethrow_interrupt(slave, source, data);
            y_axis.rethrow_interrupt(slave, source, data);
            a_axis.rethrow_interrupt(slave, source, data);

            return false; // пробросить выше
        }
    };
}

#endif
