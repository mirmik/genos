#include <doctest/doctest.h>
#include <genos/autom_schedee.h>
#include <genos/fork.h>
#include <genos/schedee_api.h>

namespace
{
    void no_op(void *, int *) {}

    class blocking_parent : public genos::schedee
    {
    public:
        genos::schedee *child_to_finish = nullptr;
        int displacements = 0;

        blocking_parent() : schedee(nullptr) {}

        void execute() override {}
        void finalize() override {}

        int displace() override
        {
            displacements++;
            if (child_to_finish != nullptr)
            {
                genos::__schedee_final(child_to_finish);
                genos::schedee_manager_step();
                child_to_finish = nullptr;
            }
            genos::force_set_current_schedee(this);
            return 0;
        }
    };

    void
    prepare_child(genos::schedee &child, genos::schedee &parent, int exit_code)
    {
        child.parent = &parent;
        child.exit_code = exit_code;
        child.preserve_zombie_state();
        child.start();
    }

    void finish_child(genos::schedee &child)
    {
        genos::__schedee_final(&child);
        genos::schedee_manager_step();
    }

    int destruction_count = 0;

    void count_destruction(genos::schedee *)
    {
        destruction_count++;
    }
}

TEST_CASE("waitpid reaps a child that exited before wait")
{
    genos::schedee_manager_init();
    blocking_parent parent;
    genos::autom_schedee child(no_op, nullptr);
    prepare_child(child, parent, 17);
    int child_pid = child.pid;
    finish_child(child);

    CHECK_EQ(child.sch_state, genos::schedee_state::zombie);
    genos::force_set_current_schedee(&parent);
    CHECK_EQ(genos::waitpid(child_pid), 17);
    CHECK_FALSE(child.schedee_list_lnk.is_linked());
    CHECK_EQ(parent.displacements, 0);

    CHECK_EQ(genos::waitpid(child_pid), -1);
}

TEST_CASE("waitpid wakes when a running child exits")
{
    genos::schedee_manager_init();
    blocking_parent parent;
    genos::autom_schedee child(no_op, nullptr);
    prepare_child(child, parent, 23);
    int child_pid = child.pid;
    parent.child_to_finish = &child;
    genos::force_set_current_schedee(&parent);

    CHECK_EQ(genos::waitpid(child_pid), 23);
    CHECK_EQ(parent.displacements, 1);
    CHECK_EQ(parent.sch_state, genos::schedee_state::run);
    CHECK_FALSE(child.schedee_list_lnk.is_linked());
}

TEST_CASE("waitpid rejects unknown and unrelated children")
{
    genos::schedee_manager_init();
    blocking_parent parent;
    blocking_parent other_parent;
    genos::autom_schedee child(no_op, nullptr);
    prepare_child(child, other_parent, 3);

    genos::force_set_current_schedee(&parent);
    CHECK_EQ(genos::waitpid(child.pid), -1);
    CHECK_EQ(genos::waitpid(12345), -1);
    CHECK_EQ(parent.sch_state, genos::schedee_state::stop);
    CHECK_EQ(parent.displacements, 0);

    child.remove_without_zombie_state();
    genos::__schedee_final(&child);
    genos::schedee_manager_step();
}

TEST_CASE("waitpid reaps multiple children independently")
{
    genos::schedee_manager_init();
    blocking_parent parent;
    genos::autom_schedee first(no_op, nullptr);
    genos::autom_schedee second(no_op, nullptr);
    prepare_child(first, parent, 11);
    prepare_child(second, parent, 22);
    int first_pid = first.pid;
    int second_pid = second.pid;

    genos::__schedee_final(&first);
    genos::__schedee_final(&second);
    genos::schedee_manager_step();

    genos::force_set_current_schedee(&parent);
    CHECK_EQ(genos::waitpid(second_pid), 22);
    CHECK_EQ(genos::waitpid(first_pid), 11);
    CHECK_EQ(genos::waitpid(second_pid), -1);
    CHECK_EQ(genos::waitpid(first_pid), -1);
}

TEST_CASE("waitpid_without_displace arms wait and later reaps")
{
    genos::schedee_manager_init();
    blocking_parent parent;
    genos::autom_schedee child(no_op, nullptr);
    prepare_child(child, parent, 31);
    int child_pid = child.pid;
    parent.start();
    genos::force_set_current_schedee(&parent);

    CHECK_EQ(genos::waitpid_without_displace(child_pid), 0);
    CHECK_EQ(parent.sch_state, genos::schedee_state::wait_schedee);

    finish_child(child);
    CHECK_EQ(parent.sch_state, genos::schedee_state::run);

    genos::force_set_current_schedee(&parent);
    CHECK_EQ(genos::waitpid_without_displace(child_pid), 31);
    CHECK_EQ(parent.future, 0);
    CHECK_EQ(genos::waitpid_without_displace(child_pid), -1);
}

TEST_CASE("reaping invokes the child destructor callback once")
{
    genos::schedee_manager_init();
    destruction_count = 0;
    blocking_parent parent;
    genos::autom_schedee child(no_op, nullptr, count_destruction);
    prepare_child(child, parent, 5);
    int child_pid = child.pid;
    finish_child(child);

    genos::force_set_current_schedee(&parent);
    CHECK_EQ(genos::waitpid(child_pid), 5);
    CHECK_EQ(destruction_count, 1);
    CHECK_EQ(genos::waitpid(child_pid), -1);
    CHECK_EQ(destruction_count, 1);
}

TEST_CASE("reaping a zombie parent detaches its running children")
{
    genos::schedee_manager_init();
    destruction_count = 0;
    blocking_parent grandparent;
    genos::autom_schedee parent(no_op, nullptr);
    genos::autom_schedee grandchild(no_op, nullptr, count_destruction);
    prepare_child(parent, grandparent, 7);
    prepare_child(grandchild, parent, 8);
    int parent_pid = parent.pid;

    finish_child(parent);
    CHECK_EQ(parent.sch_state, genos::schedee_state::zombie);

    genos::force_set_current_schedee(&grandparent);
    CHECK_EQ(genos::waitpid(parent_pid), 7);
    CHECK_EQ(grandchild.parent, nullptr);
    CHECK(grandchild.remove_without_zombie_state_flag());

    grandchild.kill();
    genos::schedee_manager_step();
    genos::schedee_manager_step();
    CHECK_EQ(destruction_count, 1);
}

TEST_CASE("releasing a parent also releases orphaned zombies")
{
    genos::schedee_manager_init();
    destruction_count = 0;
    blocking_parent parent;
    genos::autom_schedee child(no_op, nullptr, count_destruction);
    prepare_child(child, parent, 9);
    finish_child(child);

    parent.start();
    genos::__schedee_final(&parent);
    genos::schedee_manager_step();

    CHECK_EQ(child.parent, nullptr);
    CHECK_FALSE(child.schedee_list_lnk.is_linked());
    CHECK_EQ(destruction_count, 1);
}
