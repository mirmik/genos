#ifndef RALGO_HEIMER2_AXISCTR_H
#define RALGO_HEIMER2_AXISCTR_H

#include <initializer_list>
#include <ralgo/disctime.h>
#include <ralgo/heimer/axis_state.h>
#include <ralgo/heimer/axisctr_approval.h>
#include <ralgo/trajectory/linetraj.h>

#include <ralgo/heimer/signal_processor.h>

namespace heimer
{
    class axis_settings
    {
    public:
        axis_state *controlled = nullptr;
        int limits_enabled = 0;
        position_t backlim = 0;
        position_t forwlim = 0;
        sf_position_t sfpos = {0, 0};
        double gain = 1;
    };

    class axis_controller : public signal_processor
    {
        velocity_t vel = 0.1;
        acceleration_t acc = 0.1;
        acceleration_t dcc = 0.1;

    public:
        union union_t
        {
            uint8_t flags = 0;
            struct flags_t
            {
                uint8_t operation_finished_flag : 1;
                uint8_t release_control_flag : 1;
                uint8_t dynamic_resources : 1;
                uint8_t spattern_enabled : 1;
            } f;
        } u = {};

        void (*operation_start_handler)(void *priv,
                                        axis_controller *ax) = nullptr;
        void (*operation_finish_handler)(void *priv,
                                         axis_controller *ax) = nullptr;
        void *operation_handlers_priv = nullptr;

        line_trajectory lintraj = {};
        trajectory *curtraj = nullptr;

        axisctr_approval **approvals = nullptr;
        int approvals_total = 0;

        std::vector<axis_settings> settings = {};

    public:
        axis_controller(const std::string &name);
        axis_controller(const std::string &name,
                        const std::vector<axis_state *> &states);
        axis_controller(const axis_controller &) = delete;
        axis_controller &operator=(const axis_controller &) = delete;

        void add_state(axis_state *state);

        int feedback(disctime_t time) override;
        int serve(disctime_t time) override;
        int command(int argc, char **argv, char *output, int outmax) override;
        int help(char *output, int outmax) override;
        void deinit() override;
        signal_head *iterate_left(signal_head *) override;
        signal_head *iterate_right(signal_head *) override;

        void deinit(signal_processor *sigproc);

        void set_handlers(
            void *operation_handlers_priv,
            void (*operation_start_handler)(void *priv, axis_controller *ax),
            void (*operation_finish_handler)(void *priv, axis_controller *ax));

        void set_gain(double *gain);
        void set_gain(double gain)
        {
            set_gain(&gain);
        }
        void set_gain(const std::initializer_list<double> &gain);

        void set_limits_external(double *back, double *forw);
        void set_velocity_external(double vel);
        void set_accdcc_external(double acc, double dcc);
        void set_acceleration_external(double acc);
        void set_decceleration_external(double dcc);

        double external_velocity();
        double external_acceleration();
        double external_decceleration();

        void set_controlled(axis_state **state);
        void release_controlled();

        int incmove(disctime_t current_time, const double *dist_real);
        int absmove(disctime_t current_time, const double *pos_real);
        int incmove(disctime_t current_time,
                    const std::initializer_list<double> &dist_real);
        int absmove(disctime_t current_time,
                    const std::initializer_list<double> &pos_real);
        int incmove(disctime_t current_time, double dist_real)
        {
            return incmove(current_time, &dist_real);
        }
        int absmove(disctime_t current_time, double pos_real)
        {
            return absmove(current_time, &pos_real);
        }
        int stop(disctime_t);
        int hardstop(disctime_t);

        double feedpos_external(int axno);
        double ctrpos_external(int axno);
        double ctrvel_external(int axno);

        velocity_t restore_internal_velocity_from_axstates();

    private:
        int _absmove(disctime_t curtim,
                     position_t *curpos,
                     position_t *tgtpos,
                     double extdist);

        void finish_trajectory(disctime_t time, position_t *ctrpos);
        bool on_interrupt(disctime_t) override;
        void reinit();
        void allocate_resources();
        void collect_feedpos(position_t *pos);
        void collect_feedvel(velocity_t *pos);
    };

    axis_controller *create_axis_controller(const char *name);
}

#endif
