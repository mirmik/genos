/** file */

#ifndef RALGO_HEIMER_SIGNAL_PROCESSOR_H
#define RALGO_HEIMER_SIGNAL_PROCESSOR_H

#include <igris/buffer.h>
#include <igris/datastruct/dlist.h>
#include <nos/buffer.h>
#include <ralgo/disctime.h>
#include <ralgo/heimer/signal.h>

#define SIGNAL_PROCESSOR_RETURN_OK 0
#define SIGNAL_PROCESSOR_RETURN_NOT_ACTIVE 1
#define SIGNAL_PROCESSOR_RETURN_RUNTIME_ERROR 2

#define SIGNAL_PROCESSOR_NAME_MAX_LENGTH 12

namespace heimer
{
    extern struct dlist_head signal_processor_list;
    extern bool debug_activations;

    class signal_processor
    {
    public:
        struct dlist_head list_lnk = DLIST_HEAD_INIT(list_lnk);

    private:
        std::string _name = {};

        uint8_t _leftdim = 0;
        uint8_t _rightdim = 0;

    protected:
        union union_t
        {
            uint8_t flags = 0;
            struct flags_t
            {
                uint8_t active : 1;
                uint8_t dynamic_resources : 1;
                uint8_t is_axisctr : 1;
            } f;
        } u = {};

    public:
        virtual ~signal_processor()
        {
            dlist_del(&list_lnk);
        }
        signal_processor() = default;
        signal_processor(int ldim, int rdim);
        signal_processor(const std::string &name, int ldim, int rdim);
        void rebind();
        void set_name(const std::string &name);
        bool is_axisctr()
        {
            return u.f.is_axisctr;
        }

        void set_dims(int ldim, int rdim);
        uint8_t leftdim();
        uint8_t rightdim();

        /// feedback отвечает за движение сигнала слева направо.  physical ---->
        /// virtual
        virtual int feedback(disctime_t time) = 0;

        /// serve отвечает за движение сигнала справа налево. physical <----
        /// virtual
        virtual int serve(disctime_t time) = 0;

        virtual int command(int argc, char **argv, char *output, int outmax);
        virtual int help(char *output, int outmax);
        virtual int info(char *output, int outmax);
        virtual void deinit();
        virtual signal_head *iterate_left(signal_head *);
        virtual signal_head *iterate_right(signal_head *);

        void set_leftside(signal_head **arr);
        void set_rightside(signal_head **arr);

        virtual void on_activate(disctime_t);

        /// Устройству предлогают деактивировать себя.
        /// Поведение по умолчанию вызывает
        virtual int on_deactivation_request(disctime_t);
        int deactivation_request(disctime_t, bool ignore_on_deactivate = false);

        int activate(disctime_t);
        int _deactivate(disctime_t);

        bool is_active();

        nos::buffer name();
        void release_signals();

        virtual signal_head *leftsig(int i);
        virtual signal_head *rightsig(int i);
        virtual void set_leftsig(int i, signal_head *);
        virtual void set_rightsig(int i, signal_head *);
        virtual int leftsigtype(int i);
        virtual int rightsigtype(int i);

    protected:
        /// Бросить сигнал о необходимости прерывания опраций вверх по цепочке
        /// контроллеров, предварительно вызвав on_interrupt.
        /// @param ignore_handle - не вызывать метод on_interrupt.
        void interrupt(disctime_t, bool ignore_handle);

        /// Обработка поступившего прерывания.
        /// Стандартное поведение при получении прерывания - не делать ничего
        /// @resval Если true - запретить прокидывать прерывание выше.
        virtual bool on_interrupt(disctime_t)
        {
            return false;
        }

        void set_dynamic_resources_flag(bool en);
        bool is_dynamic_resources();

        friend class signal_head;
    };

    int signal_processors_count();
    void signal_processors_list_reinit();
    int
    command_signal_processors(int argc, char **argv, char *output, int maxsize);

    signal_processor *signal_processor_get_by_name(const char *name);
}

#endif
