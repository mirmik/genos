#ifndef RALGO_HEIMER_SIGNAL_H
#define RALGO_HEIMER_SIGNAL_H

#include <igris/compiler.h>
#include <igris/datastruct/dlist.h>

#include <errno.h>
#include <ralgo/disctime.h>
#include <string>

#define SIGNAL_NAME_MAX_LENGTH 12

namespace heimer
{
    class signal_processor;
    extern struct dlist_head signals_list;

    class signal_head
    {
    public:
        struct dlist_head list_lnk = DLIST_HEAD_INIT(list_lnk);
        std::string name = "undef";
        uint8_t type = 0;

        /// Количество ссылок. Каждый контроллер, имеющий указатель
        /// на этот сигнал должен инкрементировать счётчик и сбросить при
        /// отпуске ссылки.
        int16_t refs = 0;

        /// Процессор, который сейчас контолирует этот сигнал.
        /// Активируется при активации контроллера и отключается в ином случае.
        signal_processor *current_controller = nullptr;

        /// Процессор, который слушает этот сигнал.
        /// Устанавливается во время биндинга и потом не меняется.
        signal_processor *listener = nullptr;

        uint8_t sorting_mark =
            0; /// < Метка для алгоритма сортировки (см. executor)

    public:
        signal_head() = default;
        signal_head(const signal_head &) = delete;
        signal_head &operator=(const signal_head &) = delete;
        signal_head(signal_head &&) = delete;
        signal_head &operator=(signal_head &&) = delete;

        virtual ~signal_head()
        {
            dlist_del(&list_lnk);
        }
        signal_head(uint8_t type);
        signal_head(const std::string &name, uint8_t type)
            : name(name), type(type)
        {
        }
        void set_name(const std::string &name);

        virtual int info(char *buffer, int maxsize) = 0;
        int ctrinfo(char *buffer, int maxsize);

        void init(const std::string &name, uint8_t type);
        void rebind();
        void deinit();

        int attach_listener(signal_processor *);
        void attach_possible_controller(signal_processor *);
        int detach_listener(signal_processor *);
        void detach_possible_controller(signal_processor *);

        virtual int command_v(int argc, char **argv, char *output, int maxsize);

        int activate(signal_processor *proc, disctime_t);
        int deactivate(signal_processor *proc, disctime_t);

        /// Распространить прерывание вверх по цепочке контроллеров.
        void provide_interrupt(disctime_t);
    };

    int signals_count();
    void signal_head_list_reinit();
    signal_head *signal_get_by_name(const char *name);
}

#endif
