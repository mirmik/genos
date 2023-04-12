#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <genos/ktimer.h>
#include <genos/resource/resource_table.h>
#include <igris/container/dlist.h>
#include <igris/osinter/ctrobj.h>
#include <igris/osinter/wait.h>
#include <string>

#define SCHEDEE_PRIORITY_TOTAL 3
#define SCHEDEE_FDMAX 5
#define SCHEDEE_USE_PARENT_GID 1

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
        schedee *parent = nullptr;
        const char *_mnemo = "undefined";
        void (*signal_handler)(int sig) = nullptr;
        void (*destructor)(schedee *sched) = nullptr;

        // union
        //{
        //     struct ctrobj ctr;
        //     struct waiter waiter;
        // };

        igris::dlist_node control_lnk;
        struct waiter waiter;
        genos::ktimer ktimer;
        intptr_t future;

        uint8_t prio = SCHEDEE_PRIORITY_TOTAL - 1;
        schedee_state sch_state = schedee_state::stop;

        uint16_t pid = 0;
        uint16_t gid = 0;
        igris::dlist_node schedee_list_lnk = {};

        union
        {
            int local_errno;
            uint8_t syslock_counter_save;
        };

        // Ресурсы должны принадлежать процессу специального вида
        // (процесс-пользователь). всем schedee ресурсы не нужны.
        genos::resource_table restbl;
        // struct navigation_block * navblock;

    private:
        union u_s
        {
            uint8_t flags = 0;
            struct f_s
            {
                uint8_t runned : 1;
                uint8_t has_context : 1;
                uint8_t dynamic_heap : 1;
                uint8_t killed : 1;
                uint8_t is_process : 1;
                uint8_t remove_without_zombie_state : 1;
            } f;
        } u;

    public:
        schedee(void (*destructor)(schedee *sched));
        virtual ~schedee() = default;

        genos::resource_table &resource_table()
        {
            return restbl;
        }

        void set_mnemo(const char *str)
        {
            _mnemo = str;
        }

        bool is_process()
        {
            return u.f.is_process;
        }

        void kill();

        virtual void execute() = 0;
        virtual void finalize() = 0;
        virtual int displace() = 0;

        void start();
        void stop();

        const char *mnemo() const
        {
            return _mnemo;
        }
        std::string info();

        void set_priority(uint8_t prio)
        {
            this->prio = prio;
        }

        uint8_t priority()
        {
            return prio;
        }

        void set_dynamic_heap_flag()
        {
            u.f.dynamic_heap = 1;
        }
        void set_has_context_flag()
        {
            u.f.has_context = 1;
        }
        void set_is_process_flag()
        {
            u.f.is_process = 1;
        }
        void set_runned_flag()
        {
            u.f.runned = 1;
        }
        void set_killed_flag()
        {
            u.f.killed = 1;
        }

        bool dynamic_heap_flag()
        {
            return u.f.dynamic_heap;
        }
        bool has_context_flag()
        {
            return u.f.has_context;
        }
        bool runned_flag()
        {
            return u.f.runned;
        }
        bool killed_flag()
        {
            return u.f.killed;
        }
        bool remove_without_zombie_state_flag()
        {
            return u.f.remove_without_zombie_state;
        }

        void signal_received(int sig);

        void copy_open_resources_from(schedee *sch);
    };

    extern igris::dlist<schedee, &schedee::schedee_list_lnk> schedee_list;

    schedee *current_schedee();
    void force_set_current_schedee(genos::schedee *sch);

    void schedee_manager_init();
    void schedee_manager_step();

    void schedee_start(schedee *sch);
    void schedee_stop(schedee *sch);
    void schedee_pause(schedee *sch);
    void schedee_deinit(schedee *sch);
    int schedee_get_free_openres(schedee *sch, struct openres **res);

    const char *schedee_state_to_sting(genos::schedee_state state);
}

__BEGIN_DECLS
void __schedule__();
__END_DECLS

#endif