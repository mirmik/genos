#include <chrono>
#include <ralgo/cnc/planner.h>
#include <ralgo/cnc/util.h>

#pragma GCC optimize("Ofast")
void cnc::planner::cleanup()
{
    blocks->clear();
    // shifts->clear();
    // shifts->reset();
    blocks->reset();
    active_block = nullptr;
    active = blocks->head_index();
    state = 0;
    // std::fill(velocities.begin(), velocities.end(), 0);
    std::fill(accelerations.begin(), accelerations.end(), 0);
    need_to_reevaluate = false;
    state = 0;
    waited = 0;
}

bool cnc::planner::is_dda_overflow_detected()
{
    return dda_counter_overflow_error_detected;
}

void cnc::planner::disable_frequency_protection()
{
    _frequency_protection = false;
}

void cnc::planner::set_start_operation_handle(igris::delegate<void> dlg)
{
    _start_operation_handle = dlg;
}

void cnc::planner::force_skip_all_blocks()
{
    blocks->clear();
    active_block = nullptr;
    active = blocks->head_index();
    state = 0;
    std::fill(accelerations.begin(), accelerations.end(), 0);
    // memset(velocities, 0, sizeof(velocities));
    // memset(dda_counters, 0, sizeof(dda_counters));
}

// void cnc::planner::set_current_velocity(const std::vector<cnc_float_type>
// &vel)
// {
//     for (int i = 0; i < _total_axes; ++i)
//     {
//         velocities[i] = vel[i];
//     }
// }

// void cnc::planner::set_current_velocity(
//     const std::array<cnc_float_type, NMAX_AXES> &vel)
// {
//     velocities = vel;
// }

void cnc::planner::update_triggers()
{
    revolver->update_triggers();
}

void cnc::planner::set_dim(int axes)
{
    _total_axes = axes;
}

void cnc::planner::reset_iteration_counter()
{
    iteration_counter = 0;
}

cnc::planner::planner(igris::ring<cnc::planner_block> *blocks,
                      // igris::ring<cnc::control_shift> *shifts,
                      cnc::revolver *revolver)
    : blocks(blocks), revolver(revolver)
{
    std::fill(accelerations.begin(), accelerations.end(), 0);
    assert(revolver);
}

int cnc::planner::block_index(planner_block *it)
{
    return blocks->index_of(it);
}

void cnc::planner::fixup_postactive_blocks()
{
    while (blocks->tail_index() != active)
    {
        if (!blocks->tail().is_active_or_postactive(iteration_counter))
            blocks->pop();

        else
            break;
    }
}

bool cnc::planner::has_postactive_blocks()
{
    system_lock();
    auto ret = active != blocks->tail_index();
    system_unlock();
    return ret;
}

int cnc::planner::count_of_postactive()
{
    system_lock();
    auto ret = blocks->distance(active, blocks->tail_index());
    system_unlock();
    return ret;
}

void cnc::planner::change_active_block()
{
    if (info_mode)
    {
        ralgo::info("planner: change_active_block");
    }

    system_lock();

    if (active_block)
        active = blocks->fixup_index(active + 1);

    int head = blocks->head_index();
    system_unlock();

    if (active_block == nullptr && active != head)
    {
        _start_operation_handle();
    }

    if (active == head)
    {
        active_block = nullptr;
        return;
    }

    active_block = &blocks->get(active);

    assert(active_block->blockno == waited);
    waited++;

    active_block->shift_timestampes(iteration_counter);
}

// const std::array<cnc_float_type, NMAX_AXES> &cnc::planner::current_velocity()
// {
//     return velocities;
// }

void cnc::planner::evaluate_accelerations()
{
    if (active_block)
        active_block->assign_accelerations(
            accelerations.data(), _total_axes, iteration_counter);
    else
    {
        for (int i = 0; i < _total_axes; ++i)
            accelerations[i] = 0;
    }

    for (int i = blocks->tail_index(); i != active;
         i = blocks->fixup_index(i + 1))
        blocks->get(i).append_accelerations(
            accelerations.data(), _total_axes, iteration_counter);
}

void cnc::planner::set_pause_mode(bool en)
{
    pause = en;
}

int cnc::planner::serve(bool prevent_small_rooms)
{
    (void)prevent_small_rooms;

    if (pause)
        return 0;

    system_lock();
    // bool shifts_empty = shifts->empty();
    bool revolver_empty = revolver->is_empty();
    bool blocks_empty = blocks->empty();
    if (revolver_empty && blocks_empty && active_block == nullptr &&
        in_operation)
    {
        in_operation = false;
        revolver->cleanup();
        final_shift_pushed();
        system_unlock();
        return 0;
    }
    system_unlock();

    system_lock();
    int room = revolver->room();
    system_unlock();

    if (blocks_empty && active_block == nullptr)
        return 0;

    while (room)
    {
        // Планируем поведение револьвера на несколько циклов вперёд
        // попутно инкрементируя модельное время.
        auto [fin, iterations] = iteration();
        in_operation = true;

        room -= 1;

        if (fin)
            return fin;
    }

    return 0;
}

/// В этой фазе расчитывается программе револьвера
/// на основе интегрирования ускорений и скоростей.
void cnc::planner::iteration_planning_phase(size_t iter)
{
    if (revolver)
        revolver->revolver_task_ring.emplace(accelerations, iter);

    // nos::println(
    //     "add_task", accelerations, iteration_counter, iteration_counter +
    //     iter);

    /* while (iter--)
     {
         revolver_t mask, step = 0, dir = 0;
         for (int i = 0; i < _total_axes; ++i)
         {
             mask = (1 << i);

             dda_counters[i] += velocities[i] +         // delta +
                                accelerations[i] * 0.5; // delta_sqr_div_2;

             // check frequency correctness
             if (_frequency_protection)
             {
                 int gears_per_counter = dda_counters[i] / gears[i];
                 assert(gears_per_counter >= -1 && gears_per_counter <= 1);
             }

             if (dda_counters[i] > gears_high_trigger[i])
             {
                 dda_counters[i] -= gears[i];
                 dir |= mask;
                 step |= mask;
                 // if (dda_counters[i] >= gears[i])
                 // {
                 //     dda_counter_overflow_error_detected = true;
                 // }
             }
             else if (dda_counters[i] < -gears_high_trigger[i])
             {
                 dda_counters[i] += gears[i];
                 dir |= mask;
                 // if (dda_counters[i] <= gears[i])
                 // {
                 //     dda_counter_overflow_error_detected = true;
                 // }
             }
             velocities[i] += accelerations[i]; // * delta;
         }
         system_lock();
         shifts->emplace(dir, step);
         system_unlock();

         iteration_counter++;
     }*/
    iteration_counter += iter;
}

int64_t nanoseconds_summury = 0;

std::pair<int, size_t> cnc::planner::iteration()
{
    size_t room = 10000;
    (void)room;

    //    auto stime = std::chrono::steady_clock::now();

    if (active_block == nullptr && !has_postactive_blocks())
    {
        bool empty = blocks->empty();
        if (empty)
        {
            return {1, 0};
        }

        // change_active_block может установить active_block,
        // поэтому значение проверяется повторно
        change_active_block();
        if (active_block == nullptr)
            return {1, 0};

        room = 1;
        need_to_reevaluate = true;
    }

    if (active_block)
    {
        // // room = 1;
        if (active_block->is_accel(iteration_counter))
        {
            int counter_to_active =
                active_block->acceleration_before_ic - iteration_counter;
            room = std::min(room, (size_t)counter_to_active);
            need_to_reevaluate = true;
        }
        else if (active_block->is_flat(iteration_counter))
        {
            int counter_to_active =
                active_block->deceleration_after_ic - iteration_counter;
            room = std::min(room, (size_t)counter_to_active);
            need_to_reevaluate = true;
        }
        else if (active_block->is_decel(iteration_counter))
        {
            int counter_to_active =
                active_block->block_finish_ic - iteration_counter;
            room = std::min(room, (size_t)counter_to_active);
            need_to_reevaluate = true;
        }

        if (room == 0)
            room = 1;

        if (state == 0)
        {
            if (!active_block->is_accel(iteration_counter))
            {
                // go to flat
                // int counter_to_active =
                //     active_block->deceleration_after_ic - iteration_counter;
                // room = std::min(room, (size_t)counter_to_active);

                // need_to_reevaluate = true;
                state = 1;
                // room = 1;
            }
        }

        else
        {
            if (!active_block->is_active(iteration_counter))
            {
                // go to decel
                // int counter_to_decel =
                //     active_block->deceleration_after_ic - iteration_counter;
                // room = std::min(room, (size_t)counter_to_decel);

                change_active_block();
                need_to_reevaluate = true;
                state = 0;
                // room = 1;
            }
        }
    }

    for (int i = blocks->tail_index(); i != active;
         i = blocks->fixup_index(i + 1))
    {
        if (!blocks->get(i).is_active_or_postactive(iteration_counter))
        {
            need_to_reevaluate = true;
            // int counter_to_finish =
            //    blocks->get(i).block_finish_ic - iteration_counter;
            // room = std::min(room, (size_t)counter_to_finish);
            room = 1;
        }
        else
        {
            int counter_to_finish =
                blocks->get(i).block_finish_ic - iteration_counter;
            room = std::min(room, (size_t)counter_to_finish);
        }
    }

    if (need_to_reevaluate)
        reevaluate_accelerations();

    // Это лишнее?
    // if (active_block == nullptr && !has_postactive_blocks())
    //    return 1;

    // room = 1;
    iteration_planning_phase(room);
    // if (room != 1)
    //     nos::println(room);

    // auto ftime = std::chrono::steady_clock::now();
    // nanoseconds_summury +=
    //     std::chrono::duration_cast<std::chrono::nanoseconds>(ftime - stime)
    //         .count();

    // nos::println("summary", nanoseconds_summury);

    return {0, room};
}

void cnc::planner::reevaluate_accelerations()
{
    // ralgo::info("reevaluate_accelerations");
    fixup_postactive_blocks();
    evaluate_accelerations();
    need_to_reevaluate = false;
}

void cnc::planner::set_axes_count(int total)
{
    _total_axes = total;
    revolver->gears_fill(1000);
    update_triggers();
}

void cnc::planner::set_gears(const igris::array_view<cnc_float_type> &arr)
{
    revolver->set_gears(arr.data(), arr.size());
    update_triggers();
}

std::array<cnc_float_type, NMAX_AXES> cnc::planner::get_gears()
{
    return revolver->get_gears();
}

size_t cnc::planner::get_total_axes()
{
    return _total_axes;
}

size_t cnc::planner::total_axes()
{
    return _total_axes;
}

void cnc::planner::set_gear(int index, cnc_float_type val)
{
    system_lock();
    revolver->set_gear(index, val);
    update_triggers();
    system_unlock();
}

void cnc::planner::clear()
{
    blocks->clear();
    std::fill(accelerations.begin(), accelerations.end(), 0);
    // std::fill(velocities.begin(), velocities.end(), 0);
    active_block = nullptr;
    active = blocks->head_index();
    need_to_reevaluate = true;
    state = 0;
    waited = 0;
    revolver->clear();
    change_active_block();
}

void cnc::planner::clear_for_stop()
{
    blocks->set_last_index(active);
    blocks->clear();
    std::fill(accelerations.begin(), accelerations.end(), 0);
    // std::fill(velocities.begin(), velocities.end(), 0);
    active_block = nullptr;
    active = blocks->head_index();
    need_to_reevaluate = true;
    state = 0;
    waited = 0;
    revolver->clear_no_velocity_drop();
    change_active_block();
}

// void cnc::planner::clear_queue()
//{
// blocks->set_last_index(active);
// clear();
//}

void cnc::planner::alarm_stop()
{
    system_lock();
    blocks->clear();
    std::fill(accelerations.begin(), accelerations.end(), 0);
    // std::fill(velocities.begin(), velocities.end(), 0);
    active_block = nullptr;
    active = blocks->head_index();
    need_to_reevaluate = true;
    state = 0;
    waited = 0;
    change_active_block();
    // shifts->clear();
    revolver->clear();
    system_unlock();
}

bool cnc::planner::is_not_halt()
{
    igris::syslock_guard lock;
    return !revolver->is_empty() || active_block != nullptr ||
           has_postactive_blocks() || !blocks->empty();
}

bool cnc::planner::is_halt()
{
    return !is_not_halt();
}
#pragma GCC reset_options