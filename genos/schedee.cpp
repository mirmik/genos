#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <genos/signal.h>
#include <igris/container/dlist.h>
#include <igris/sync/syslock.h>
#include <string>

igris::dlist<genos::schedee, &genos::schedee::schedee_list_lnk>
    genos::schedee_list;

struct dlist_head runlist[SCHEDEE_PRIORITY_TOTAL];
DLIST_HEAD(unstoplist);
DLIST_HEAD(waitlist);
DLIST_HEAD(finallist);

uint16_t pid_counter = 0;

genos::schedee *__current_schedee = NULL;
genos::schedee *genos::current_schedee()
{
    return __current_schedee;
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

genos::schedee::schedee(void (*destructor)(schedee *sched))
    : destructor(destructor)
{
    ctrobj_init(&ctr, CTROBJ_SCHEDEE_LIST);
    this->prio = SCHEDEE_PRIORITY_TOTAL - 1;
    sch_state = schedee_state::stop;
    syslock_counter_save = 0;
    parent = current_schedee();
    local_errno = 0;
}

void genos::schedee_manager_init()
{
    for (int i = 0; i < SCHEDEE_PRIORITY_TOTAL; ++i)
        dlist_init(&runlist[i]);

    dlist_init(&waitlist);
    dlist_init(&unstoplist);
    dlist_init(&finallist);
}

void genos::schedee_start(genos::schedee *sch)
{
    // TODO : Здесь должна быть какая-то защита от попытки оперировать таймером,
    //        находящимся в ожидании таймера.

    system_lock();
    sch->sch_state = schedee_state::run;
    sch->ctr.type = CTROBJ_SCHEDEE_LIST;

    dlist_move_tail(&sch->ctr.lnk, &unstoplist);

    system_unlock();
}

void genos::schedee_stop(genos::schedee *sch)
{
    // TODO : Здесь должна быть какая-то защита от попытки оперировать таймером,
    //        находящимся в ожидании таймера.

    system_lock();
    sch->sch_state = schedee_state::stop;
    sch->ctr.type = CTROBJ_SCHEDEE_LIST;
    dlist_move_tail(&sch->ctr.lnk, &waitlist);
    system_unlock();
}

void __schedee_run(genos::schedee *sch)
{
    dlist_move_tail(&sch->ctr.lnk, &runlist[sch->prio]);
}

void genos::__schedee_final(genos::schedee *sch)
{
    system_lock();
    sch->sch_state = schedee_state::final;
    dlist_move_tail(&sch->ctr.lnk, &finallist);
    system_unlock();
}

void __schedee_execute(genos::schedee *sch)
{
    __current_schedee = sch;
    sch->u.f.runned = 1;
    sch->execute();
}

void schedee_notify_finalize(genos::schedee *sch)
{
    if (sch->parent)
        sch->parent->signal_handler(SIGCHLD);
}

void genos::schedee_manager_step()
{
    genos::schedee *sch;

    // Отрабатываем логику завершения процессов.
    system_lock();
    while (!dlist_empty(&finallist))
    {
        sch = dlist_first_entry(&finallist, genos::schedee, ctr.lnk);
        dlist_del_init(&sch->ctr.lnk);

        system_unlock();

        sch->sch_state = schedee_state::zombie;
        sch->finalize();

        schedee_notify_finalize(sch);

        system_lock();
    }
    system_unlock();

    // Перемещаем процессы, объявленные запущенными в основные листы.
    system_lock();
    while (!dlist_empty(&unstoplist))
    {
        sch = dlist_first_entry(&unstoplist, genos::schedee, ctr.lnk);
        __schedee_run(sch);
    }
    system_unlock();

    for (int priolvl = 0; priolvl < SCHEDEE_PRIORITY_TOTAL; priolvl++)
    {
        if (!dlist_empty(&runlist[priolvl]))
        {
            sch = dlist_first_entry(&runlist[priolvl], genos::schedee, ctr.lnk);

            if (sch->u.f.killed)
            {
                __schedee_final(sch);
                return;
            }

            dlist_move_tail(&sch->ctr.lnk, &runlist[priolvl]);
            __schedee_execute(sch);

            return;
        }
    }

    // Nobody to run
    return;
}

void genos::schedee_deinit(genos::schedee *sch)
{
    dlist_del_init(&sch->ctr.lnk);
    dlist_del_init(&sch->schedee_list_lnk);
}

void genos::schedee::start()
{
    genos::schedee_list.move_back(*this);
    this->pid = generate_new_pid();
    genos::schedee_start(this);
}

void genos::schedee::stop()
{
    genos::schedee_stop(this);
}

const char *schedee_state_to_sting(genos::schedee_state state)
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

int info_cmd(const nos::argv &args, nos::ostream &os)
{
    for (auto &sch : genos::schedee_list)
    {
        nos::print_to(os, " mnemo: ");
        nos::print_to(os, sch.mnemo());
        nos::print_to(os, " pid: ");
        nos::print_to(os, std::to_string(sch.pid));
        nos::print_to(os, " state: ");
        nos::print_to(os, schedee_state_to_sting(sch.sch_state));
        nos::print_to(os, " prio: ");
        nos::print_to(os, std::to_string(sch.prio));
        nos::println_to(os);
    }
    return 0;
}

nos::executor genos::schedee_manager_executor{
    {nos::command{"ps", "proccess information", info_cmd}}};