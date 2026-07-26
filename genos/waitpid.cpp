#include <genos/fork.h>
#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <igris/sync/syslock.h>

namespace
{
    enum class child_wait_result
    {
        error,
        pending,
        reaped,
    };

    struct child_wait
    {
        child_wait_result result = child_wait_result::error;
        int exit_code = -1;
    };

    child_wait
    wait_or_reap_child(genos::schedee *parent, intptr_t pid, bool arm_wait)
    {
        if (parent == nullptr || pid <= 0)
            return {};

        genos::schedee *child = nullptr;
        int exit_code = -1;

        system_lock();
        for (auto &candidate : genos::schedee_list)
        {
            if (candidate.parent == parent && candidate.pid == pid)
            {
                child = &candidate;
                break;
            }
        }

        if (child == nullptr)
        {
            system_unlock();
            return {};
        }

        if (child->sch_state == genos::schedee_state::zombie)
        {
            exit_code = child->exit_code;
            if (parent->future == pid)
                parent->future = 0;
            child->parent = nullptr;
            child->control_lnk.unlink();
            child->schedee_list_lnk.unlink();
            system_unlock();

            genos::schedee_release(child);

            return {child_wait_result::reaped, exit_code};
        }

        if (!arm_wait)
        {
            system_unlock();
            return {};
        }

        parent->future = pid;
        parent->sch_state = genos::schedee_state::wait_schedee;
        parent->control_lnk.unlink();
        system_unlock();
        return {child_wait_result::pending, 0};
    }
}

int genos::waitpid(intptr_t pid)
{
    genos::schedee *parent = current_schedee();
    child_wait wait = wait_or_reap_child(parent, pid, true);

    if (wait.result == child_wait_result::error)
        return -1;
    if (wait.result == child_wait_result::reaped)
        return wait.exit_code;

    current_schedee_displace();

    wait = wait_or_reap_child(parent, pid, false);
    if (wait.result != child_wait_result::reaped)
        return -1;
    return wait.exit_code;
}

int genos::waitpid_without_displace(intptr_t pid)
{
    child_wait wait = wait_or_reap_child(current_schedee(), pid, true);
    if (wait.result == child_wait_result::error)
        return -1;
    return wait.exit_code;
}
