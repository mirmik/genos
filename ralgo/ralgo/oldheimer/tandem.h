#ifndef RALGO_HEIMER_TANDEM_H
#define RALGO_HEIMER_TANDEM_H

#include <ralgo/oldheimer/control.h>
#include <ralgo/oldheimer/multiax.h>

namespace heimer
{
    template <class P, class V> class tandem : public control_node
    {
    public:
        virtual_axis_node<P, V> master;
        virtual_axis_node<P, V> slave;

        axis_node<P, V> *master_controlled;
        axis_node<P, V> *slave_controlled;

        float koeff;

        constexpr tandem(const char *mnemo,
                         const char *master_mnemo,
                         const char *slave_mnemo,
                         axis_node<P, V> *_master,
                         axis_node<P, V> *_slave,
                         float koeff)
            : control_node(mnemo), master(master_mnemo, this),
              slave(slave_mnemo, this), master_controlled(_master),
              slave_controlled(_slave), koeff(koeff)
        {
        }

        void feedback()
        {
            float mx = master_controlled->feedpos;
            float mv = master_controlled->feedspd;

            float sx = slave_controlled->feedpos;
            float sv = slave_controlled->feedspd;

            master.feedpos = mx;
            master.feedspd = mv;

            slave.feedpos = sx - koeff * mx;
            slave.feedspd = sv - koeff * mv;
        };

        void serve_impl() override
        {
            float mx = master.ctrpos;
            float mv = master.ctrspd;

            float sx = slave.ctrpos;
            float sv = slave.ctrspd;

            master_controlled->ctrpos = mx;
            master_controlled->ctrspd = mv;

            slave_controlled->ctrpos = sx + koeff * mx;
            slave_controlled->ctrspd = sv + koeff * mv;
        };

        control_node *iterate(control_node *slt) override
        {
            if (slt == nullptr)
                return master_controlled;

            if (slt == master_controlled)
                return slave_controlled;

            return nullptr;
        }

        int on_activate() override
        {
            master.flags |= HEIM_IS_ACTIVE;
            slave.flags |= HEIM_IS_ACTIVE;
            return 0;
        }

        int on_deactivate() override
        {
            master.flags &= ~HEIM_IS_ACTIVE;
            slave.flags &= ~HEIM_IS_ACTIVE;
            return 0;
        }

        void print_info() override
        {
            nos::println("mu: feedpos:", master_controlled->feedpos);
            nos::println("mu: feedspd:", master_controlled->feedspd);
            nos::println("mu: ctrpos:", master_controlled->ctrpos);
            nos::println("mu: ctrspd:", master_controlled->ctrspd);

            nos::println("su: feedpos:", slave_controlled->feedpos);
            nos::println("su: feedspd:", slave_controlled->feedspd);
            nos::println("su: ctrpos:", slave_controlled->ctrpos);
            nos::println("su: ctrspd:", slave_controlled->ctrspd);

            nos::println("mc: feedpos:", master.feedpos);
            nos::println("mc: feedspd:", master.feedspd);
            nos::println("mc: ctrpos:", master.ctrpos);
            nos::println("mc: ctrspd:", master.ctrspd);

            nos::println("sc: feedpos:", slave.feedpos);
            nos::println("sc: feedspd:", slave.feedspd);
            nos::println("sc: ctrpos:", slave.ctrpos);
            nos::println("sc: ctrspd:", slave.ctrspd);
        }

        virtual bool on_interrupt(control_node *slave_node,
                                  control_node *source,
                                  interrupt_args *data)
        {
            if (data->code() == HEIMER_INTERRUPT_TYPE_CONTROL_UPDATE)
            {
                feedback();
            }

            master.rethrow_interrupt(slave_node, source, data);
            slave.rethrow_interrupt(slave_node, source, data);

            return false; // пробросить выше
        }
    };
}

#endif
