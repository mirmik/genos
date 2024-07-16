#include <ralgo/cnc/revolver.h>

cnc::revolver::revolver() : revolver_task_ring(100)
{
    std::fill(dda_counters_fixed.begin(), dda_counters_fixed.end(), 0);
    std::fill(velocities_fixed.begin(), velocities_fixed.end(), 0);
    current_revolver_task = nullptr;
}

bool cnc::revolver::is_empty()
{
    igris::syslock_guard guard;
    return revolver_task_ring.empty();
}

robo::stepper **cnc::revolver::get_steppers()
{
    return steppers;
}

void cnc::revolver::set_steppers(robo::stepper **steppers_table, int size)
{
    steppers = steppers_table;
    steppers_total = size;
}

// int cnc::revolver::queue_size()
// {
//     int size;

//     system_lock();
//     size = shifts_ring->avail();
//     system_unlock();

//     return size;
// }

void cnc::revolver::current_steps(int64_t *steps)
{
    system_lock();
    for (int i = 0; i < steppers_total; ++i)
        steps[i] = steppers[i]->steps_count();
    system_unlock();
}

std::vector<int64_t> cnc::revolver::current_steps()
{
    std::vector<int64_t> vec(steppers_total);
    system_lock();
    for (int i = 0; i < steppers_total; ++i)
        vec[i] = steppers[i]->steps_count();
    system_unlock();
    return vec;
}

std::vector<int64_t> cnc::revolver::current_steps_no_lock()
{
    std::vector<int64_t> vec(steppers_total);
    for (int i = 0; i < steppers_total; ++i)
        vec[i] = steppers[i]->steps_count();
    return vec;
}

int cnc::revolver::room()
{
    // Операция взятия оставшегося места над кольцевым буфером
    // требует сравнения head и tail, изменяемых в разных потоках,
    // поэтому не является атомарной.
    system_lock();
    int ret = revolver_task_ring.room();
    system_unlock();

    return ret;
}

void cnc::revolver::cleanup()
{
    system_lock();
    std::fill(dda_counters_fixed.begin(), dda_counters_fixed.end(), 0);
    std::fill(velocities_fixed.begin(), velocities_fixed.end(), 0);
    revolver_task_ring.clear();
    current_revolver_task = nullptr;
    system_unlock();
}

// void cnc::revolver::push(uint16_t step, uint16_t dir)
// {
//     // Добавление данных в очередь не требует блокировки,
//     // потому что ring_head lockfree на добавление и чтение,
//     // если unsigned int атомарен.

//     // shifts_ring->emplace(step, dir);
// }

void cnc::revolver::serve()
{
    // if (shifts_ring->empty())
    //     return;

    if (current_revolver_task == nullptr && !revolver_task_ring.empty())
    {
        current_revolver_task = &revolver_task_ring.tail();
        counter = current_revolver_task->counter;

        // nos::println("REVOLVER TASK START");
        // nos::println("step_start", current_revolver_task->step_start);
        // nos::println("step_end", current_revolver_task->step_end);
        // nos::println("accelerations", current_revolver_task->accelerations);
        // std::cout.flush();
    }

    if (current_revolver_task == nullptr)
    {
        return;
    }

    // if (iteration_counter >= 3)
    // {
    //     exit(0);
    // }

    for (int i = 0; i < steppers_total; ++i)
    {
        // mask = (1 << i);

        // if (current_revolver_task->accelerations[i] < 0)
        // {
        //     nos::println(current_revolver_task->accelerations[i]);
        // }
        dda_counters_fixed[i] +=
            velocities_fixed[i] + //* delta +
            (current_revolver_task->accelerations_fixed[i] >>
             1); //* delta_sqr_div_2;

        // nos::println("dda_counters[i]", dda_counters[i]);

        // check frequency correctness
        // int gears_per_counter = dda_counters[i] / gears[i];
        int64_t double_gears = gears_fixed[i] + gears_fixed[i];
        assert(dda_counters_fixed[i] >= -double_gears &&
               dda_counters_fixed[i] <= double_gears);

        auto &stepper = *steppers[i];
        if (dda_counters_fixed[i] > gears_high_trigger_fixed[i])
        {
            dda_counters_fixed[i] -= gears_fixed[i];
            stepper.inc();
            //    dir |= mask;
            //    step |= mask;
            // if (dda_counters[i] >= gears[i])
            // {
            //     dda_counter_overflow_error_detected = true;
            // }
        }
        else if (dda_counters_fixed[i] < -gears_high_trigger_fixed[i])
        {
            stepper.dec();
            dda_counters_fixed[i] += gears_fixed[i];
            //    dir |= mask;
            // if (dda_counters[i] <= gears[i])
            // {
            //     dda_counter_overflow_error_detected = true;
            // }
        }
        velocities_fixed[i] +=
            current_revolver_task->accelerations_fixed[i]; // * delta;
    }

    // auto &shift = shifts_ring->tail();
    //(void)shift;

    // for (int i = 0; i < steppers_total; ++i)
    // {
    //     revolver_t mask = 1 << i;
    //     bool step = shift.step & mask;

    //     if (!step)
    //         continue;

    //     bool dir = shift.direction & mask;
    //     auto &stepper = *steppers[i];

    //     if (step)
    //     {
    //         if (dir)
    //             stepper.inc();
    //         else
    //             stepper.dec();
    //     }
    //     else
    //     {
    //         stepper.no_action();
    //     }
    // }

    // shifts_ring->move_tail_one();
    counter--;

    if (counter == 0)
    {
        revolver_task_ring.move_tail_one();
        current_revolver_task = nullptr;
    }

    return;
}

void cnc::revolver::clear()
{
    nos::println("CLEAR");
    system_lock();
    // shifts_ring->clear();
    revolver_task_ring.clear();
    current_revolver_task = nullptr;
    counter = 0;
    std::fill(dda_counters_fixed.begin(), dda_counters_fixed.end(), 0);
    std::fill(velocities_fixed.begin(), velocities_fixed.end(), 0);
    system_unlock();
}

void cnc::revolver::clear_no_velocity_drop()
{
    nos::println("CLEAR_NO_VELOCITY_DROP");
    system_lock();
    // shifts_ring->clear();
    revolver_task_ring.clear();
    current_revolver_task = nullptr;
    counter = 0;
    // std::fill(dda_counters.begin(), dda_counters.end(), 0);
    // std::fill(velocities.begin(), velocities.end(), 0);
    system_unlock();
}