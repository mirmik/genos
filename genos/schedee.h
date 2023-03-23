#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <genos/ktimer.h>
#include <genos/resource/resource_table.h>
#include <igris/osinter/ctrobj.h>
#include <igris/osinter/wait.h>
#include <string>

#define SCHEDEE_PRIORITY_TOTAL 3
#define SCHEDEE_FDMAX 5
#define SCHEDEE_USE_PARENT_GID 1

extern struct dlist_head schedee_list;

namespace genos
{
    enum class schedee_state
    {
        run,
        wait,
        wait_schedee,
        stop,
        final,
        zombie
    };

    class schedee
    {
    public:
        schedee *parent;
        const char *_mnemo = "undefined";
        void (*signal_handler)(int sig);
        void (*destructor)(schedee *sched);

        union
        {
            struct ctrobj ctr;
            struct waiter waiter;
            genos::ktimer ktimer;
        };

        uint8_t prio;
        schedee_state sch_state;

        uint16_t pid;
        struct dlist_head schedee_list_lnk;

        union
        {
            int local_errno;
            uint8_t syslock_counter_save;
        };

        // Ресурсы должны принадлежать процессу специального вида
        // (процесс-пользователь). всем schedee ресурсы не нужны.
        genos::resource_table restbl;
        // struct navigation_block * navblock;

    public:
        union u_s
        {
            uint8_t flags;
            struct f_s
            {
                uint8_t runned : 1;
                uint8_t can_displace : 1;
                uint8_t has_context : 1;
                uint8_t dynamic_heap : 1;
                uint8_t killed : 1;
            } f;
        } u;

    public:
        schedee(void (*destructor)(schedee *sched) = nullptr);

        void set_mnemo(const char *str)
        {
            _mnemo = str;
        }

        virtual void execute() = 0;
        virtual void finalize() = 0;
        virtual int displace() = 0;

        void start();
        void stop();

        virtual ~schedee() = default;

        const char *mnemo() const
        {
            return _mnemo;
        }
        std::string info();
    };

    schedee *current_schedee();

    void schedee_manager_init();
    void schedee_manager_step();

    void schedee_start(schedee *sch);
    void schedee_stop(schedee *sch);
    void schedee_pause(schedee *sch);
    void schedee_deinit(schedee *sch);
    int schedee_get_free_openres(schedee *sch, struct openres **res);
}

__BEGIN_DECLS
void __schedule__();
__END_DECLS

#endif