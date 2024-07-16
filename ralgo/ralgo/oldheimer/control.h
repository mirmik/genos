#ifndef HEIMER_CONTROL2_H
#define HEIMER_CONTROL2_H

#include <igris/datastruct/dlist.h>
#include <igris/shell/conscmd.h>

#include <ralgo/oldheimer/interrupt_args.h>

#include <nos/print.h>

namespace heimer
{
    extern dlist_head control_node_list;

    enum ControlError : int16_t
    {
        HEIM_ERR_IN_OPERATE = -1,
        HEIM_ERR_IS_BUSY = -2,
        HEIM_ERR_IS_NOACTIVE = -3,
        HEIM_ERR_IS_CONTROL_FAULT = -4,
        HEIM_ERR_IS_ALARM = -5,
        HEIM_ERR_IS_PARTED = -6,
    };

    enum ControlFlags : uint16_t
    {
        HEIM_IS_ACTIVE = (1 << 0),
        HEIM_IS_CONTROLLED = (1 << 4),
        HEIM_IS_ALARM = (1 << 1),
        //		HEIM_MASTER        = (1 << 2),
        //		HEIM_SLAVE         = (1 << 3)
        HEIM_IS_MULTICONTROLLED = (1 << 5)
    };

    class control_node
    {
    public:
        dlist_head control_node_list_lnk =
            DLIST_HEAD_INIT(control_node_list_lnk);
        uint16_t flags = 0;

    private:
        const char *_mnemo = nullptr;
        int alarm_code = 0;

        // контроллер, взявший управление над объектом
        // или последний контроллер, бравший управление
        // в multicontroller mode
        control_node *controller = nullptr;

    public:
        const control_node *last_controller()
        {
            return controller;
        }

        virtual ~control_node() = default;
        constexpr control_node(const char *mnemo) : _mnemo(mnemo)
        {
            dlist_add(&control_node_list_lnk, &control_node_list);
        }
        control_node(const control_node &) = delete;
        control_node &operator=(const control_node &) = delete;

        void set_alarm(int errcode)
        {
            flags |= HEIM_IS_ALARM;
            this->alarm_code = errcode;
        }

        // итератор подчинённых устройств
        virtual control_node *iterate(control_node *)
        {
            return nullptr;
        }

        // Итератор контроллирующих устройств
        virtual control_node *controllers_iterate(control_node *it)
        {
            if (it == nullptr)
                return controller;
            else
                return nullptr;
        }

        // Вызывается при после успешной активации устройства
        int activate();
        int deactivate();
        int take_control();
        int release_control();

        void serve();
        virtual void serve_impl() = 0;

        bool is_active()
        {
            return flags & HEIM_IS_ACTIVE;
        }

        bool is_controlled()
        {
            return flags & HEIM_IS_CONTROLLED;
        }

        bool is_multicontrolled()
        {
            return flags & HEIM_IS_MULTICONTROLLED;
        }

        bool is_alarmed()
        {
            return flags & HEIM_IS_ALARM;
        }

        void set_multicontrolled_mode()
        {
            flags |= HEIM_IS_MULTICONTROLLED;
        }

        const char *mnemo() const
        {
            return _mnemo;
        }

        void set_mnemo(const char *str)
        {
            _mnemo = str;
        }

        virtual void print_info()
        {
            nos::println("info");
        }

        virtual int internal_command(int, char **)
        {
            return -1;
        }

        void
        rethrow_interrupt(control_node *slave, // источник, переславший сигнал
                          control_node *source, // изначальный источник сигнала
                          interrupt_args *data)
        {
            on_interrupt_common(slave, source, data);
        }

    protected:
        // вызывается при взятии внешнего управления нодом
        virtual int on_external_take(control_node *)
        {
            return 0;
        }

        // вызывается при отпускании внешнего управления
        virtual int on_external_release(control_node *)
        {
            return 0;
        }

        virtual int on_activate()
        {
            return 0;
        }
        virtual int on_deactivate()
        {
            return 0;
        }

        // обратное уведомления о событиях
        // если возвращает истину, распространение сообщения
        // на контроллеры предупреждается
        virtual bool
        on_interrupt(control_node *slave, // источник, переславший сигнал
                     control_node *source, // изначальный источник сигнала
                     interrupt_args *data) = 0;

        // обратное уведомления о событиях
        void on_interrupt_common(
            control_node *slave, // источник, переславший сигнал
            control_node *source, // изначальный источник сигнала
            interrupt_args *data);

        void throw_interrupt(interrupt_args *interrupt);

        static control_node *vector_iterate(control_node **bit,
                                            control_node **eit,
                                            control_node *it);
    };

    extern igris::console_command info_node_commands[];
}

#endif
