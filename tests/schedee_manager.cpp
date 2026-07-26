#include <cstdint>
#include <doctest/doctest.h>
#include <genos/autom_schedee.h>
#include <iostream>
#include <string>

void foo_exec(void *data, int *state)
{
    (void)state;
    *(int *)(data) = 1;
}

static int destructor_calls = 0;

void count_destruction(genos::schedee *)
{
    destructor_calls++;
}

struct execution_record
{
    int id;
    int *order;
    int *count;
};

void record_execution(void *data, int *)
{
    auto *record = static_cast<execution_record *>(data);
    record->order[(*record->count)++] = record->id;
    genos::current_schedee()->kill();
}

TEST_CASE("schedee_manager execution")
{
    genos::schedee_manager_init();
    int state = 0;
    genos::autom_schedee sch(foo_exec, &state);
    sch.start();

    genos::schedee_manager_step();
    CHECK(state == 1);
    CHECK_EQ(genos::schedee_list.size(), 1);
}

TEST_CASE("schedee priority rejects values outside the runlist")
{
    int state = 0;
    genos::autom_schedee sch(foo_exec, &state);

    CHECK(sch.set_priority(0));
    CHECK_EQ(sch.priority(), 0);

    CHECK(sch.set_priority(SCHEDEE_PRIORITY_TOTAL - 1));
    CHECK_EQ(sch.priority(), SCHEDEE_PRIORITY_TOTAL - 1);

    CHECK_FALSE(sch.set_priority(SCHEDEE_PRIORITY_TOTAL));
    CHECK_EQ(sch.priority(), SCHEDEE_PRIORITY_TOTAL - 1);

    CHECK_FALSE(sch.set_priority(UINT8_MAX));
    CHECK_EQ(sch.priority(), SCHEDEE_PRIORITY_TOTAL - 1);
}

TEST_CASE("schedee manager runs valid priorities in order")
{
    genos::schedee_manager_init();
    int order[2] = {};
    int count = 0;
    execution_record low_record{2, order, &count};
    execution_record high_record{0, order, &count};
    genos::autom_schedee low(record_execution, &low_record);
    genos::autom_schedee high(record_execution, &high_record);

    CHECK(high.set_priority(0));
    low.start();
    high.start();

    genos::schedee_manager_step();
    CHECK_EQ(count, 1);
    CHECK_EQ(order[0], 0);

    genos::schedee_manager_step();
    CHECK_EQ(count, 2);
    CHECK_EQ(order[1], 2);

    genos::schedee_manager_step();
    genos::schedee_manager_step();
}

TEST_CASE("externally owned schedee finalizes without a destructor callback")
{
    genos::schedee_manager_init();
    int state = 0;
    genos::autom_schedee sch(foo_exec, &state);

    sch.start();
    sch.kill();
    genos::schedee_manager_step();
    CHECK_EQ(sch.sch_state, genos::schedee_state::final);

    genos::schedee_manager_step();
    CHECK_EQ(sch.sch_state, genos::schedee_state::zombie);
    CHECK_EQ(genos::schedee_list.size(), 0);
}

TEST_CASE("schedee destructor callback runs once")
{
    genos::schedee_manager_init();
    destructor_calls = 0;
    int state = 0;
    genos::autom_schedee sch(foo_exec, &state, count_destruction);

    sch.start();
    sch.kill();
    genos::schedee_manager_step();
    genos::schedee_manager_step();
    genos::schedee_manager_step();

    CHECK_EQ(destructor_calls, 1);
}

TEST_CASE("all schedee states have stable names")
{
    CHECK_EQ(
        std::string(genos::schedee_state_to_string(genos::schedee_state::run)),
        "RUN");
    CHECK_EQ(
        std::string(genos::schedee_state_to_string(genos::schedee_state::wait)),
        "WAIT");
    CHECK_EQ(std::string(genos::schedee_state_to_string(
                 genos::schedee_state::wait_schedee)),
             "WAIT_SCHEDEE");
    CHECK_EQ(
        std::string(genos::schedee_state_to_string(genos::schedee_state::stop)),
        "STOP");
    CHECK_EQ(std::string(
                 genos::schedee_state_to_string(genos::schedee_state::final)),
             "FINAL");
    CHECK_EQ(std::string(
                 genos::schedee_state_to_string(genos::schedee_state::zombie)),
             "ZOMBIE");
    CHECK_EQ(std::string(genos::schedee_state_to_string(
                 static_cast<genos::schedee_state>(UINT8_MAX))),
             "UNKNOWN");
}
