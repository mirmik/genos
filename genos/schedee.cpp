#include <array>
#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <genos/signal.h>
#include <igris/container/dlist.h>
#include <igris/dprint.h>
#include <igris/sync/syslock.h>
#include <string>

__attribute__((init_priority(140)))
igris::dlist<genos::schedee, &genos::schedee::schedee_list_lnk>
    genos::schedee_list;

std::array<igris::dlist<genos::schedee, &genos::schedee::control_lnk>,
           SCHEDEE_PRIORITY_TOTAL>
    runlist;
igris::dlist<genos::schedee, &genos::schedee::control_lnk> unstoplist;
igris::dlist<genos::schedee, &genos::schedee::control_lnk> waitlist;
igris::dlist<genos::schedee, &genos::schedee::control_lnk> finallist;

uint16_t pid_counter = 0;

genos::schedee *__current_schedee = NULL;
genos::schedee *genos::current_schedee()
{
    return __current_schedee;
}

void genos::force_set_current_schedee(genos::schedee *sch)
{
    __current_schedee = sch;
}

uint16_t generate_new_pid()
{
    bool founded = false;
    do
    {
        pid_counter++;
        if (pid_counter == 0)
            pid_counter++;

        founded = false;
        for (auto &sch : genos::schedee_list)
        {
            if (pid_counter == sch.pid)
            {
                founded = true;
                break;
            }
        }

    } while (founded);

    return pid_counter;
}

bool is_schedee_manager_lists_correct__DEBUG()
{
    system_lock();
    for (int i = 0; i < SCHEDEE_PRIORITY_TOTAL; ++i)
    {
        if (!runlist[i].is_correct())
            abort();
    }

    if (!waitlist.is_correct())
        abort();

    if (!unstoplist.is_correct())
        abort();

    if (!finallist.is_correct())
        abort();

    system_unlock();
    return true;
}

genos::schedee::schedee(void (*destructor)(schedee *sched))
    : destructor(destructor)
{
    this->destructor = destructor;
    u.flags = 0;
    restbl.clear();
    local_errno = 0;
    syslock_counter_save = 0;
    pid = 0;
    gid = 0;
    sch_state = schedee_state::stop;
    prio = SCHEDEE_PRIORITY_TOTAL - 1;
    parent = nullptr;
    signal_handler = nullptr;
    _mnemo = "undefined";
    u.f.remove_without_zombie_state = 1;
}

void genos::schedee::kill()
{
    if (u.f.killed)
        return;
    u.f.killed = 1;
}

void genos::schedee_manager_init()
{
    for (auto &list : runlist)
        list.clear();

    waitlist.clear();
    unstoplist.clear();
    finallist.clear();
}

void genos::schedee_start(genos::schedee *sch)
{
    system_lock();
    sch->sch_state = schedee_state::run;
    // sch->ctr.type = CTROBJ_SCHEDEE_LIST;
    unstoplist.move_back(*sch);
    system_unlock();
}

void genos::schedee_stop(genos::schedee *sch)
{
    system_lock();
    sch->sch_state = schedee_state::stop;
    // sch->ctr.type = CTROBJ_SCHEDEE_LIST;
    waitlist.move_back(*sch);
    system_unlock();
}

void __schedee_run(genos::schedee *sch)
{
    runlist[sch->prio].move_back(*sch);
}

void schedee_notify_finalize(genos::schedee *sch)
{
    if (sch && sch->parent)
        sch->parent->signal_received(SIGCHLD);
}

void genos::__schedee_final(genos::schedee *sch)
{
    auto tmp_current = current_schedee();
    __current_schedee = sch;
    system_lock();
    sch->sch_state = schedee_state::final;
    schedee_notify_finalize(sch);
    finallist.move_back(*sch);
    system_unlock();
    __current_schedee = tmp_current;
}

void __schedee_execute(genos::schedee *sch)
{
    __current_schedee = sch;
    sch->set_runned_flag();
    sch->execute();
}

void genos::schedee_manager_step()
{
    genos::schedee *sch;

    // Отрабатываем логику завершения процессов.
    system_lock();
    while (!finallist.empty())
    {
        sch = &finallist.front();
        sch->control_lnk.unlink();
        sch->schedee_list_lnk.unlink();

        system_unlock();

        sch->sch_state = schedee_state::zombie;
        sch->finalize();

        if (sch->remove_without_zombie_state_flag())
        {
            schedee_deinit(sch);
            sch->destructor(sch);
        }
        system_lock();
    }
    system_unlock();

    // Перемещаем процессы, объявленные запущенными в основные листы.
    system_lock();
    while (!unstoplist.empty())
    {
        sch = &unstoplist.front();
        __schedee_run(sch);
    }
    system_unlock();

    for (int priolvl = 0; priolvl < SCHEDEE_PRIORITY_TOTAL; priolvl++)
    {
        system_lock();
        if (!runlist[priolvl].empty())
        {
            sch = &runlist[priolvl].front();

            if (sch->killed_flag())
            {
                __schedee_final(sch);
                system_unlock();
                continue;
            }

            runlist[sch->prio].move_back(*sch);
            system_unlock();

            __schedee_execute(sch);
            return;
        }
        system_unlock();
    }

    // Nobody to run
    return;
}

void genos::schedee_deinit(genos::schedee *sch)
{
    sch->control_lnk.unlink();
    sch->schedee_list_lnk.unlink();
}

void genos::schedee::start()
{
    system_lock();
    if (!schedee_list_lnk.is_linked())
        genos::schedee_list.move_back(*this);
    if (this->pid == 0)
        this->pid = generate_new_pid();

    system_unlock();
    genos::schedee_start(this);
}

void genos::schedee::stop()
{
    genos::schedee_stop(this);
}

const char *genos::schedee_state_to_sting(genos::schedee_state state)
{
    switch (state)
    {
    case genos::schedee_state::run:
        return "RUN";
    case genos::schedee_state::stop:
        return "STOP";
    case genos::schedee_state::final:
        return "FINAL";
    case genos::schedee_state::zombie:
        return "ZOMBIE";
    default:
        return "UNKNOWN";
    }
}

std::string genos::schedee::info()
{
    std::string str;
    str += "pid: ";
    str += std::to_string(pid);
    str += "state: ";
    str += schedee_state_to_sting(sch_state);
    str += "prio: ";
    return str;
}

void genos::schedee::signal_received(int sig)
{
    if (sig == SIGCHLD && sch_state == schedee_state::wait_schedee)
    {
        auto *cursch = genos::current_schedee();
        int pid = cursch->future;
        if (pid == cursch->pid)
            this->start();
    }

    if (signal_handler)
        signal_handler(sig);
}

void genos::schedee::copy_open_resources_from(genos::schedee *sch)
{
    auto &other_res = sch->resource_table();
    auto &my_res = this->resource_table();
    for (size_t i = 0; i < other_res.size(); i++)
    {
        auto &res = other_res[i];
        my_res.open_new_as(res);
    }
}