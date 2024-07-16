
#include <chrono>
#include <doctest/doctest.h>
#include <mutex>
#include <thread>

#include <ralgo/cnc/interpreter.h>
#include <ralgo/cnc/planner.h>
#include <ralgo/cnc/revolver.h>
#include <ralgo/freq_invoke_imitation.h>
#include <ralgo/global_protection.h>

nos::log::logger logger;
using namespace std::chrono_literals;

TEST_CASE("cnc.runtest")
{
    ralgo::set_logger(&logger);
    igris::ring<cnc::planner_block> blocks{40};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({1000, 1000, 1000});
    interpreter.set_revolver_frequency(5000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {200us, [&]() { revolver.serve(); }},
    };

    CHECK_EQ(revolver.current_velocities()[0], 0);
    CHECK_EQ(revolver.current_velocities()[1], 0);
    CHECK_EQ(revolver.current_velocities()[2], 0);
    auto responce = interpreter.newline("cnc G1 X10000 F10000 M10000\n");
    CHECK_EQ(responce, "");

    auto last_block = interpreter.last_block();

    CHECK_EQ(last_block.axdist[0], 10000);
    CHECK_EQ(last_block.axdist[1], 0);
    CHECK_EQ(last_block.axdist[2], 0);
    CHECK_EQ(last_block.direction()[0], 1);
    CHECK_EQ(last_block.direction()[1], 0);
    CHECK_EQ(last_block.direction()[2], 0);
    CHECK_EQ(last_block.nominal_velocity, 10000.0 / 5000.0);
    CHECK_LT(abs(last_block.acceleration - 10000.0f / 5000.0f / 5000.0f), 1e-5);
    CHECK_EQ(last_block.fullpath, 10000);

    CHECK_EQ(steppers[0].steps(), 0);
    imitator.timeskip(500ms);

    CHECK_EQ(steppers[0].steps(), 1);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 1);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 2);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 3);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 4);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 5);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 6);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 6);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 7);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 8);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 8);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 9);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 9);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 9);
    imitator.timeskip(100ms);

    CHECK_EQ(steppers[0].steps(), 10);
    imitator.timeskip(100ms);

    CHECK_FALSE(planner.is_dda_overflow_detected());
    CHECK_EQ(interpreter.final_position()[0], 10000);
}

TEST_CASE("cnc.runtest with gears assigned one")
{
    ralgo::set_logger(&logger);
    igris::ring<cnc::planner_block> blocks{40};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};
    cnc_float_type freq = 10000.0;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {100us, [&]() { revolver.serve(); }},
    };

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({1, 1, 1});
    interpreter.set_revolver_frequency(freq);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    auto responce = interpreter.newline("cnc G1 X10000 F10000 M10000\n");
    CHECK_EQ(responce, "");

    auto last_block = interpreter.last_block();

    CHECK_EQ(last_block.axdist[0], 10000);
    CHECK_EQ(last_block.axdist[1], 0);
    CHECK_EQ(last_block.axdist[2], 0);
    CHECK_EQ(last_block.direction()[0], 1);
    CHECK_EQ(last_block.direction()[1], 0);
    CHECK_EQ(last_block.direction()[2], 0);
    CHECK_EQ(last_block.nominal_velocity, 10000.0 / freq);
    CHECK_EQ(last_block.acceleration, 10000.0f / freq / freq);
    CHECK_EQ(last_block.fullpath, 10000);

    std::vector<
        std::tuple<std::chrono::milliseconds, cnc_float_type, cnc_float_type>>
        points{
            {100ms, 0, 20},       {200ms, 40, 60},      {300ms, 190, 200},
            {400ms, 440, 450},    {500ms, 790, 800},    {600ms, 1240, 1250},
            {700ms, 1780, 1800},  {800ms, 2440, 2460},  {900ms, 3180, 3200},
            {1000ms, 4030, 4050}, {1100ms, 4980, 5000}, {1200ms, 5930, 5950},
            {1300ms, 6780, 6820}, {1400ms, 7530, 7550}, {1500ms, 8180, 8200},
            {1600ms, 8730, 8750}, {1700ms, 9180, 9200}, {1800ms, 9530, 9550},
            {1900ms, 9780, 9800}, {2000ms, 9940, 9999}, {2100ms, 9999, 10000}};

    auto lasttime = 0ns;
    for (auto &p : points)
    {
        auto steps = steppers[0].steps();
        CHECK_LE(steps, std::get<2>(p));
        CHECK_GE(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }

    CHECK_FALSE(planner.is_dda_overflow_detected());
    CHECK_EQ(interpreter.final_position()[0], 10000);
}

TEST_CASE("cnc.runtest with gears assigned ten")
{
    ralgo::set_logger(&logger);
    igris::ring<cnc::planner_block> blocks{40};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({10, 10, 10});
    interpreter.set_revolver_frequency(5000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {200us, [&]() { revolver.serve(); }},
    };

    auto responce = interpreter.newline("cnc G1 X10000 F10000 M10000\n");
    CHECK_EQ(responce, "");

    auto last_block = interpreter.last_block();

    CHECK_EQ(last_block.axdist[0], 10000);
    CHECK_EQ(last_block.axdist[1], 0);
    CHECK_EQ(last_block.axdist[2], 0);
    CHECK_EQ(last_block.direction()[0], 1);
    CHECK_EQ(last_block.direction()[1], 0);
    CHECK_EQ(last_block.direction()[2], 0);
    CHECK_LT(abs(last_block.nominal_velocity - 10000.0f / 5000.0f), 1e-5);
    CHECK_LT(abs(last_block.acceleration - 10000.0f / 5000.0f / 5000.0f), 1e-5);
    CHECK_EQ(last_block.fullpath, 10000);

    std::vector<
        std::tuple<std::chrono::milliseconds, cnc_float_type, cnc_float_type>>
        points{{100ms, 0, 2},      {200ms, 4, 6},       {300ms, 19, 20},
               {400ms, 44, 45},    {500ms, 79, 80},     {600ms, 124, 125},
               {700ms, 178, 180},  {800ms, 244, 245},   {900ms, 318, 320},
               {1000ms, 403, 405}, {1100ms, 498, 500},  {1200ms, 593, 595},
               {1300ms, 678, 680}, {1400ms, 753, 755},  {1500ms, 818, 820},
               {1600ms, 873, 875}, {1700ms, 918, 920},  {1800ms, 953, 955},
               {1900ms, 978, 980}, {2000ms, 994, 1000}, {2100ms, 1000, 1000}};

    auto lasttime = 0ns;
    for (auto &p : points)
    {
        auto steps = steppers[0].steps();
        CHECK_LE(steps, std::get<2>(p));
        CHECK_GE(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }

    CHECK_FALSE(planner.is_dda_overflow_detected());
    CHECK_EQ(interpreter.final_position()[0], 10000);
}

TEST_CASE("cnc.runtest move and stop")
{
    igris::ring<cnc::planner_block> blocks{40};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({10, 10, 10});
    interpreter.set_revolver_frequency(10000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {100us, [&]() { revolver.serve(); }},
    };

    auto responce = interpreter.newline("cnc G1 X20000 F10000 M10000\n");
    CHECK_EQ(responce, "");

    std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>> points_0{
        {100ms, 0},
        {200ms, 5},
        {300ms, 19},
        {400ms, 44},
        {500ms, 79},
        {600ms, 124},
        {700ms, 179},
        {800ms, 244},
        {900ms, 319},
        {1000ms, 404},
        {1100ms, 499}};

    auto lasttime = 0ns;
    for (auto &p : points_0)
    {
        auto steps = steppers[0].steps();
        CHECK_EQ(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
        //        CHECK(planner.active_block != nullptr);
    }

    auto responce_1 = interpreter.newline("cmd stop\n");
    CHECK_EQ(responce_1, "");

    auto last_block = interpreter.last_block();
    CHECK_EQ(last_block.blockno, 0);
    // CHECK(fabs(last_block.axdist[0] - 5000) < 1e-5);
    CHECK_EQ(last_block.direction()[0], 1);
    CHECK_EQ(last_block.direction()[1], 0);
    CHECK_EQ(last_block.direction()[2], 0);
    CHECK(fabs(last_block.nominal_velocity - 10000.0 / 10000) < 1e-5);
    CHECK(last_block.acceleration - 10000.0 / 10000 / 10000 < 1e-5);

    std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>> points_1{
        {1101ms, 599},
        {1110ms, 599},
        {1120ms, 608},
        {1130ms, 618},
        {1140ms, 627},
        {1150ms, 637},
        {1200ms, 647},
        {1300ms, 693},
        {1400ms, 778},
        {1500ms, 853},
        {1600ms, 918},
        {1800ms, 973},
        {1900ms, 1053},
        {2000ms, 1079},
        {2100ms, 1094},
        {2200ms, 1099},
        {2300ms, 1099},
        {2400ms, 1099}};

    for (auto &p : points_1)
    {
        auto steps = steppers[0].steps();
        CHECK_EQ(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }

    CHECK_EQ(blocks.avail(), 0);
    CHECK_EQ(revolver.revolver_task_ring.avail(), 0);
    CHECK_EQ(revolver.current_revolver_task, nullptr);
    CHECK_FALSE(planner.is_dda_overflow_detected());
}

TEST_CASE("cnc.runtest with gears assigned ten")
{
    ralgo::set_logger(&logger);
    igris::ring<cnc::planner_block> blocks{40};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({10, 10, 10});
    interpreter.set_revolver_frequency(5000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {200us, [&]() { revolver.serve(); }},
    };

    auto responce = interpreter.newline("cnc G1 X10000 F10000 M10000\n");
    CHECK_EQ(responce, "");

    auto last_block = interpreter.last_block();

    CHECK_EQ(last_block.axdist[0], 10000);
    CHECK_EQ(last_block.axdist[1], 0);
    CHECK_EQ(last_block.axdist[2], 0);
    CHECK_EQ(last_block.direction()[0], 1);
    CHECK_EQ(last_block.direction()[1], 0);
    CHECK_EQ(last_block.direction()[2], 0);
    CHECK_EQ(last_block.nominal_velocity, 10000.0 / 5000.0);
    CHECK_LT(abs(last_block.acceleration - 10000.0f / 5000.0f / 5000.0f), 1e-5);
    CHECK_EQ(last_block.fullpath, 10000);

    std::vector<
        std::tuple<std::chrono::milliseconds, cnc_float_type, cnc_float_type>>
        points{{100ms, 0, 2},      {200ms, 4, 6},       {300ms, 19, 20},
               {400ms, 44, 45},    {500ms, 79, 80},     {600ms, 124, 125},
               {700ms, 178, 180},  {800ms, 244, 245},   {900ms, 318, 320},
               {1000ms, 403, 405}, {1100ms, 498, 500},  {1200ms, 593, 595},
               {1300ms, 678, 680}, {1400ms, 753, 755},  {1500ms, 818, 820},
               {1600ms, 873, 875}, {1700ms, 918, 920},  {1800ms, 953, 955},
               {1900ms, 978, 980}, {2000ms, 994, 1000}, {2100ms, 1000, 1000}};

    auto lasttime = 0ns;
    for (auto &p : points)
    {
        auto steps = steppers[0].steps();
        CHECK_LE(steps, std::get<2>(p));
        CHECK_GE(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }

    CHECK_EQ(blocks.avail(), 0);
    CHECK_EQ(revolver.revolver_task_ring.avail(), 0);
    CHECK_EQ(revolver.current_revolver_task, nullptr);
    CHECK_FALSE(planner.is_dda_overflow_detected());
    CHECK_EQ(interpreter.final_position()[0], 10000);
}

TEST_CASE("cnc.runtest stop and move")
{
    igris::ring<cnc::planner_block> blocks{40};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({10, 10, 10});
    interpreter.set_revolver_frequency(10000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {100us, [&]() { revolver.serve(); }},
    };

    std::chrono::nanoseconds lasttime = 0ns;
    {
        auto responce = interpreter.newline("cmd stop\n");
        CHECK_EQ(responce, "");

        auto last_block = interpreter.last_block();

        CHECK_EQ(last_block.axdist[0], 0);
        CHECK_EQ(last_block.axdist[1], 0);
        CHECK_EQ(last_block.axdist[2], 0);
        CHECK_EQ(last_block.direction()[0], 0);
        CHECK_EQ(last_block.direction()[1], 0);
        CHECK_EQ(last_block.direction()[2], 0);
        CHECK_EQ(last_block.nominal_velocity, 0);
        CHECK_EQ(last_block.acceleration, 0);
        CHECK_EQ(last_block.fullpath, 0);

        std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>>
            points_1{{100ms, 0}, {200ms, 0}, {300ms, 0}};

        for (auto &p : points_1)
        {
            auto steps = steppers[0].steps();
            CHECK_EQ(steps, std::get<1>(p));
            imitator.timeskip(std::get<0>(p) - lasttime);
            lasttime = std::get<0>(p);
        }

        CHECK_FALSE(planner.is_dda_overflow_detected());
    }
    {
        auto responce2 = interpreter.newline("cnc G1 X10000 F10000 M10000\n");
        CHECK_EQ(responce2, "");

        auto last_block = interpreter.last_block();

        CHECK_EQ(last_block.axdist[0], 10000);
        CHECK_EQ(last_block.axdist[1], 0);
        CHECK_EQ(last_block.axdist[2], 0);
        CHECK_EQ(last_block.direction()[0], 1);
        CHECK_EQ(last_block.direction()[1], 0);
        CHECK_EQ(last_block.direction()[2], 0);
        CHECK_EQ(last_block.nominal_velocity, 10000.0 / 10000.0);
        CHECK_LT(abs(last_block.acceleration - 10000.0f / 10000.0f / 10000.0f),
                 1e-5);
        CHECK_EQ(last_block.fullpath, 10000);

        std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>>
            points_0{{400ms, 0},
                     {500ms, 5},
                     {600ms, 19},
                     {700ms, 44},
                     {800ms, 79},
                     {900ms, 124},
                     {1000ms, 179},
                     {1100ms, 244},
                     {1200ms, 319},
                     {1300ms, 404},
                     {1400ms, 499}};

        for (auto &p : points_0)
        {
            auto steps = steppers[0].steps();
            CHECK_EQ(steps, std::get<1>(p));
            imitator.timeskip(std::get<0>(p) - lasttime);
            lasttime = std::get<0>(p);
        }

        CHECK_FALSE(planner.is_dda_overflow_detected());
    }
}

TEST_CASE("cnc.runtest move and stop and move")
{
    igris::ring<cnc::planner_block> blocks{40};
    igris::ring<cnc::control_shift> shifts{1000};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({10, 10, 10});
    interpreter.set_revolver_frequency(10000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {100us, [&]() { revolver.serve(); }},
    };

    auto responce = interpreter.newline("cmd absmove X20000 F10000 M10000\n");
    CHECK_EQ(responce, "");

    CHECK_EQ(interpreter.final_position()[0], 20000);

    std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>> points_0{
        {100ms, 0},
        {200ms, 5},
        {300ms, 19},
        {400ms, 44},
        {500ms, 79},
        {600ms, 124},
        {700ms, 179},
        {800ms, 244},
        {900ms, 319},
        {1000ms, 404},
        {1100ms, 499}};

    auto lasttime = 0ns;
    for (auto &p : points_0)
    {
        auto steps = steppers[0].steps();
        CHECK_EQ(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
        // CHECK(planner.active_block != nullptr);
    }

    CHECK_EQ(interpreter.last_block().blockno, 0);
    CHECK_EQ(interpreter.final_position()[0], 20000);

    auto responce_1 = interpreter.newline("cmd stop\n");
    CHECK_EQ(responce_1, "");

    auto last_block = interpreter.last_block();
    CHECK_EQ(last_block.blockno, 0);
    // CHECK(fabs(last_block.axdist[0] - 5000) < 1e-5);
    CHECK_EQ(last_block.direction()[0], 1);
    CHECK_EQ(last_block.direction()[1], 0);
    CHECK_EQ(last_block.direction()[2], 0);
    CHECK(fabs(last_block.nominal_velocity - 10000.0 / 10000) < 1e-5);
    CHECK(last_block.acceleration - 10000.0 / 10000 / 10000 < 1e-5);

    std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>> points_1{
        {1101ms, 599},
        {1110ms, 599},
        {1120ms, 608},
        {1130ms, 618},
        {1140ms, 627},
        {1150ms, 637},
        //{1200ms, 646},
        {1200ms, 647},
        {1300ms, 693},
        {1400ms, 778},
        {1500ms, 853},
        {1600ms, 918},
        {1800ms, 973},
        {1900ms, 1053},
        {2000ms, 1079},
        //{2000ms, 1078},
        {2100ms, 1094},
        {2200ms, 1099},
        {2300ms, 1099},
        {2400ms, 1099}};

    for (auto &p : points_1)
    {
        auto steps = steppers[0].steps();
        CHECK_EQ(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }

    CHECK_FALSE(planner.is_dda_overflow_detected());

    auto responce3 = interpreter.newline("cmd absmove X20000 F10000 M10000\n");
    CHECK_EQ(responce3, "");

    CHECK_EQ(interpreter.final_position()[0], 20000);

    std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>> points_2{
        {2500ms, 1099}, {2600ms, 1104}, {2700ms, 1118}, {2800ms, 1143},
        {2900ms, 1178}, {3000ms, 1223}, {3100ms, 1278}, {3200ms, 1343},
        {3300ms, 1418}, {3400ms, 1503}, {3500ms, 1595}, {3600ms, 1680},
        {3700ms, 1755}, {3800ms, 1820}, {3900ms, 1875}, {4000ms, 1920},
        {4100ms, 1955}, {4200ms, 1980}, {4300ms, 1995}, {4400ms, 2000},
        {4500ms, 2000}};

    for (auto &p : points_2)
    {
        auto steps = steppers[0].steps();
        CHECK_EQ(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }

    CHECK_EQ(steppers[0].steps(), 2000);
    CHECK(planner.active_block == nullptr);
    CHECK_EQ(blocks.avail(), 0);
    CHECK_EQ(revolver.revolver_task_ring.avail(), 0);
    CHECK_EQ(revolver.current_revolver_task, nullptr);
    CHECK_FALSE(planner.is_dda_overflow_detected());
}

TEST_CASE("cnc.runtest 2 blocks")
{
    igris::ring<cnc::planner_block> blocks{40};
    igris::ring<cnc::control_shift> shifts{1000};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({10, 10, 10});
    interpreter.set_revolver_frequency(10000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {100us, [&]() { revolver.serve(); }},
    };

    auto responce = interpreter.newline("cmd absmove X10000 F10000 M10000\n");
    CHECK_EQ(responce, "");
    CHECK_EQ(interpreter.final_position()[0], 10000);

    std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>> points_0{
        {100ms, 0},     {200ms, 5},     {300ms, 19},   {400ms, 44},
        {500ms, 79},    {600ms, 124},   {700ms, 179},  {800ms, 244},
        {900ms, 319},   {1000ms, 404},  {1100ms, 499}, {1200ms, 594},
        {1300ms, 679},  {1400ms, 754},  {1500ms, 819}, {1600ms, 874},
        {1700ms, 919},  {1800ms, 954},  {1900ms, 979}, {2000ms, 995},
        {2100ms, 1000}, {2200ms, 1000},
    };

    auto lasttime = 0ns;
    for (auto &p : points_0)
    {
        auto steps = steppers[0].steps();
        CHECK_EQ(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }
}

TEST_CASE("cnc.runtest 2 blocks : 2")
{
    igris::ring<cnc::planner_block> blocks{40};
    igris::ring<cnc::control_shift> shifts{1000};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({10, 10, 10});
    interpreter.set_revolver_frequency(10000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {100us, [&]() { revolver.serve(); }},
    };

    auto responce = interpreter.newline("cmd absmove X20000 F10000 M10000\n");
    CHECK_EQ(responce, "");
    CHECK_EQ(interpreter.final_position()[0], 20000);

    std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>> points_0{
        {100ms, 0},     {200ms, 5},     {300ms, 19},    {400ms, 44},
        {500ms, 79},    {600ms, 124},   {700ms, 179},   {800ms, 244},
        {900ms, 319},   {1000ms, 404},  {1100ms, 499},  {1200ms, 599},
        {1300ms, 699},  {1400ms, 799},  {1500ms, 899},  {1600ms, 999},
        {1700ms, 1099}, {1800ms, 1199}, {1900ms, 1299}, {2000ms, 1399},
        {2100ms, 1499}, {2200ms, 1594},
    };

    auto lasttime = 0ns;
    for (auto &p : points_0)
    {
        auto steps = steppers[0].steps();
        CHECK_EQ(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }
}

TEST_CASE("cnc.runtest 2 blocks : 3")
{
    igris::ring<cnc::planner_block> blocks{40};
    igris::ring<cnc::control_shift> shifts{1000};
    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    cnc::interpreter interpreter(&blocks, &planner, &revolver, nullptr);
    robo::stepper steppers[3];
    robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({10, 10, 10});
    interpreter.set_revolver_frequency(10000);
    revolver.set_steppers(steppers_ptrs, 3);
    ralgo::global_protection = false;

    ralgo::freq_invoke_imitation imitator = {
        {1ms, [&]() { planner.serve(); }},
        {100us, [&]() { revolver.serve(); }},
    };

    interpreter.newline("cmd absmove X10000 F10000 M10000\n");
    interpreter.newline("cmd absmove X20000 F10000 M10000\n");
    CHECK_EQ(interpreter.final_position()[0], 20000);

    std::vector<std::tuple<std::chrono::milliseconds, cnc_float_type>> points_0{
        {100ms, 0},     {200ms, 5},     {300ms, 19},    {400ms, 44},
        {500ms, 79},    {600ms, 124},   {700ms, 179},   {800ms, 244},
        {900ms, 319},   {1000ms, 404},  {1100ms, 499},  {1200ms, 599},
        {1300ms, 699},  {1400ms, 799},  {1500ms, 899},  {1600ms, 999},
        {1700ms, 1099}, {1800ms, 1199}, {1900ms, 1299}, {2000ms, 1399},
        {2100ms, 1499}, {2200ms, 1594},
    };

    auto lasttime = 0ns;
    for (auto &p : points_0)
    {
        auto steps = steppers[0].steps();
        CHECK_EQ(steps, std::get<1>(p));
        imitator.timeskip(std::get<0>(p) - lasttime);
        lasttime = std::get<0>(p);
    }
}