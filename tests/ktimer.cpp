#include <doctest/doctest.h>
#include <genos/ktimer.h>
#include <igris/sync/syslock.h>

static int a = 0;

void action(void *, genos::ktimer *)
{
    a = 1;
}

TEST_CASE("ktimer invoke")
{
    genos::ktimer_manager_init();
    a = 0;
    genos::ktimer tim(action, NULL, 0, 100);
    tim.plan();

    CHECK(!tim.check(0));
    CHECK(tim.check(100));

    genos::ktimer_manager_step(0);
    CHECK_EQ(a, 0);
    genos::ktimer_manager_step(100);
    CHECK_EQ(a, 1);

    tim.deinit();
}

struct replan_context
{
    int calls = 0;
    int callback_lock_count = -1;
};

void replan_once(void *arg, genos::ktimer *tim)
{
    auto *ctx = static_cast<replan_context *>(arg);
    ctx->callback_lock_count = syslock_counter();
    ctx->calls++;
    if (ctx->calls == 1)
        tim->replan();
}

TEST_CASE("ktimer callback runs unlocked and can replan itself")
{
    genos::ktimer_manager_init();
    replan_context ctx;
    genos::ktimer tim(replan_once, &ctx, 0, 10);
    tim.plan();

    genos::ktimer_manager_step(10);
    CHECK_EQ(ctx.calls, 1);
    CHECK_EQ(ctx.callback_lock_count, 0);
    CHECK(tim.planned());
    CHECK_EQ(tim.start, 10);

    genos::ktimer_manager_step(19);
    CHECK_EQ(ctx.calls, 1);

    genos::ktimer_manager_step(20);
    CHECK_EQ(ctx.calls, 2);
    CHECK_FALSE(tim.planned());
}

struct ordered_callback
{
    int id;
    int *order;
    int *count;
    genos::ktimer *timer_to_unplan = nullptr;
};

void record_timer(void *arg, genos::ktimer *)
{
    auto *ctx = static_cast<ordered_callback *>(arg);
    ctx->order[(*ctx->count)++] = ctx->id;
    if (ctx->timer_to_unplan != nullptr)
        ctx->timer_to_unplan->unplan();
}

TEST_CASE("ktimers with equal deadlines keep planning order")
{
    genos::ktimer_manager_init();
    int order[2] = {};
    int count = 0;
    ordered_callback first_ctx{1, order, &count};
    ordered_callback second_ctx{2, order, &count};
    genos::ktimer first(record_timer, &first_ctx, 0, 10);
    genos::ktimer second(record_timer, &second_ctx, 0, 10);

    first.plan();
    second.plan();
    genos::ktimer_manager_step(10);

    CHECK_EQ(count, 2);
    CHECK_EQ(order[0], 1);
    CHECK_EQ(order[1], 2);
    CHECK_FALSE(first.planned());
    CHECK_FALSE(second.planned());
}

TEST_CASE("ktimer callback can unplan another expired timer")
{
    genos::ktimer_manager_init();
    int order[2] = {};
    int count = 0;
    ordered_callback first_ctx{1, order, &count};
    ordered_callback second_ctx{2, order, &count};
    genos::ktimer first(record_timer, &first_ctx, 0, 10);
    genos::ktimer second(record_timer, &second_ctx, 0, 10);
    first_ctx.timer_to_unplan = &second;

    first.plan();
    second.plan();
    genos::ktimer_manager_step(10);

    CHECK_EQ(count, 1);
    CHECK_EQ(order[0], 1);
    CHECK_FALSE(first.planned());
    CHECK_FALSE(second.planned());
    CHECK_EQ(genos::ktimer_manager_planed_count(), 0);
}
