#ifndef MOVECTR_H
#define MOVECTR_H

#include <genos/coop_schedee.h>
#include <genos/schedee_api.h>

class MoveController
{
protected:
    bool _removeflag = false;
    volatile bool _runstate = false;
    pid_t schpid = 0;

public:
    //Каждый контроллер имеет исполняемый процесс.
    // switchScheduler::process_switch* proc;
    virtual void remove_impl() = 0;
    void remove()
    {
        _removeflag = true;
    }

    MoveController() {}

    virtual int exec() = 0;

    bool runned()
    {
        return _runstate;
    }

    void kill()
    {
        _runstate = false;
        if (schpid)
            genos::kill(schpid);
        schpid = 0;
    }

    int starter()
    {
        return exec();
    }

    void start_schedee()
    {
        auto exec = igris::delegate<int>(&MoveController::starter, this);
        auto schedee = genos::create_coop_schedee(
            exec.get_extfunc(), exec.get_object(), nullptr, 1024);
        schedee->start();
        schpid = schedee->pid;
    }
};

#endif