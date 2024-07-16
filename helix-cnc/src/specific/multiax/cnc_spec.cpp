
#include <igris/util/bug.h>
#include <nos/print.h>
#include <quadgen.h>
#include <ralgo/cnc/revolver.h>

#define TOTAL_CTRL_AXES 10

struct abpin
{
    zillot::stm32::pin a;
    zillot::stm32::pin b;
};

abpin control_pins[]{{
                         // 1
                         {GPIOG, 1 << 14},
                         {GPIOG, 1 << 15},
                     },
                     {
                         // 2
                         {GPIOG, 1 << 10},
                         {GPIOG, 1 << 13},
                     },
                     {
                         // 3
                         {GPIOC, 1 << 8},
                         {GPIOC, 1 << 9},
                     },
                     {
                         // 4
                         {GPIOG, 1 << 8},
                         {GPIOG, 1 << 7},
                     },
                     {
                         // 5
                         {GPIOG, 1 << 2},
                         {GPIOG, 1 << 3},
                     },
                     {
                         // 6
                         {GPIOD, 1 << 9},
                         {GPIOD, 1 << 10},
                     },
                     {
                         // 7
                         {GPIOE, 1 << 13},
                         {GPIOE, 1 << 14},
                     },
                     {
                         // 8
                         {GPIOE, 1 << 11},
                         {GPIOE, 1 << 12},
                     },
                     {
                         // 9
                         {GPIOF, 1 << 13},
                         {GPIOF, 1 << 14},
                     },
                     {
                         // 8
                         {GPIOB, 1 << 1},
                         {GPIOB, 1 << 2},
                     }};

abpin feedback_pins[]{{
                          // 1
                          {GPIOA, 1 << 5}, // TIM2 CH1 AF1
                          {GPIOB, 1 << 3}, // TIM2 CH2 AF1
                      },
                      {
                          // 2
                          {GPIOB, 1 << 4}, // TIM3 CH1 AF2
                          {GPIOB, 1 << 5}, // TIM3 CH2 AF2
                      },
                      {
                          // 3
                          {GPIOA, 1 << 8}, // TIM1 CH1 AF1
                          {GPIOA, 1 << 9}, // TIM1 CH2 AF1
                      },
                      {
                          // 4
                          {GPIOG, 1 << 12}, // LPTIM1_IN1 AF1 // not used
                          {GPIOG, 1 << 11}, // LPTIM1_IN2 AF1
                      },
                      {
                          // 5
                          {GPIOD, 1 << 12}, // TIM4 CH1 AF2
                          {GPIOD, 1 << 13}, // TIM4 CH2 AF2
                      },
                      {
                          // 6
                          {GPIOC, 1 << 6}, // TIM8 CH1 AF3
                          {GPIOC, 1 << 7}, // TIM8 CH2 AF3
                      },
                      {
                          // 7
                          {GPIOF, 1 << 11}, // TIM24 CH1 AF14
                          {GPIOF, 1 << 12}, // TIM24 CH2 AF14
                      },
                      {
                          // 8
                          {GPIOB, 1 << 10}, // LPTIM1_IN1 af3
                          {GPIOD, 1 << 11}, // LPTIM2_IN2 af3
                      },
                      {
                          // 9
                          {GPIOF, 1 << 0}, // TIM23 CH1 AF13
                          {GPIOF, 1 << 1}, // TIM23 CH2 AF13
                      }};

// static_assert(sizeof(control_pins) == sizeof(feedback_pins));
static_assert(sizeof(control_pins) ==
              sizeof(zillot::stm32::pin) * 2 * TOTAL_CTRL_AXES);

quadgen_stm32_2 physax[TOTAL_CTRL_AXES];
robo::quadgen *control_quadgens[TOTAL_CTRL_AXES];

void init_control_pins()
{
    nos::print("Contol pins initialization ... ");
    for (int i = 0; i < std::size(control_pins); ++i)
    {
        control_pins[i].a.setup(GPIO_MODE_OUTPUT);
        control_pins[i].b.setup(GPIO_MODE_OUTPUT);
        physax[i] = {control_pins[i].a, control_pins[i].b};
        control_quadgens[i] = &physax[i];
    }
    nos::println("Ok");
}

extern igris::ring<cnc::control_shift> control_ring;

//double double_gears = revolver.gears[i] * 2;                           \
        //assert(revolver.dda_counters[i] >= -double_gears &&                    \
        //       revolver.dda_counters[i] <= double_gears);                      \

#define CONTROL_BOTTOM_LEVEL_PHASE(i)                                          \
    {                                                                          \
        revolver.dda_counters_fixed[i] +=                                      \
            revolver.velocities_fixed[i] +                                     \
            (revolver.current_revolver_task->accelerations_fixed[i] >> 1);     \
                                                                               \
        auto &stepper = physax[i];                                             \
        if (revolver.dda_counters_fixed[i] >                                   \
            revolver.gears_high_trigger_fixed[i])                              \
        {                                                                      \
            revolver.dda_counters_fixed[i] -= revolver.gears_fixed[i];         \
            ++stepper.counter;                                                 \
            switch (stepper._curcode)                                          \
            {                                                                  \
            case 0b00:                                                         \
                stepper._curcode = 0b01;                                       \
                break;                                                         \
            case 0b01:                                                         \
                stepper._curcode = 0b11;                                       \
                break;                                                         \
            case 0b10:                                                         \
                stepper._curcode = 0b00;                                       \
                break;                                                         \
            case 0b11:                                                         \
                stepper._curcode = 0b10;                                       \
                break;                                                         \
            }                                                                  \
            stepper.apply_code(stepper._curcode);                              \
        }                                                                      \
        else if (revolver.dda_counters_fixed[i] <                              \
                 -revolver.gears_high_trigger_fixed[i])                        \
        {                                                                      \
            --stepper.counter;                                                 \
            switch (stepper._curcode)                                          \
            {                                                                  \
            case 0b00:                                                         \
                stepper._curcode = 0b10;                                       \
                break;                                                         \
            case 0b01:                                                         \
                stepper._curcode = 0b00;                                       \
                break;                                                         \
            case 0b10:                                                         \
                stepper._curcode = 0b11;                                       \
                break;                                                         \
            case 0b11:                                                         \
                stepper._curcode = 0b01;                                       \
                break;                                                         \
            }                                                                  \
            stepper.apply_code(stepper._curcode);                              \
            revolver.dda_counters_fixed[i] += revolver.gears_fixed[i];         \
        }                                                                      \
        revolver.velocities_fixed[i] +=                                        \
            revolver.current_revolver_task->accelerations_fixed[i];            \
    }

extern cnc::revolver revolver;
// void control_bottom_level()
// {
//     revolver.serve();
// }
#pragma GCC optimize("Ofast")
void control_bottom_level()
{
    if (revolver.current_revolver_task == nullptr &&
        !revolver.revolver_task_ring.empty())
    {
        revolver.current_revolver_task = &revolver.revolver_task_ring.tail();
        revolver.counter = revolver.current_revolver_task->counter;
    }

    if (revolver.current_revolver_task == nullptr)
    {
        return;
    }

    CONTROL_BOTTOM_LEVEL_PHASE(0);
    CONTROL_BOTTOM_LEVEL_PHASE(1);
    CONTROL_BOTTOM_LEVEL_PHASE(2);
    CONTROL_BOTTOM_LEVEL_PHASE(4);

    revolver.counter--;

    if (revolver.counter == 0)
    {
        revolver.revolver_task_ring.move_tail_one();
        revolver.current_revolver_task = nullptr;
    }

    return;
}
#pragma GCC reset_options

// #pragma GCC optimize("O3")
// void control_bottom_level()
// {
//     if (control_ring.empty())
//         return;

//     auto &shift = control_ring.tail();
//     revolver_t steps = shift.step;

//     unsigned int j;
//     while ((j = __builtin_ffs(steps)))
//     {
//         unsigned int i = j - 1;
//         revolver_t mask = 1 << i;
//         steps &= ~mask;

//         bool dir = shift.direction & mask;
//         auto &stepper = physax[i];

//         if (dir)
//             stepper.inc();
//         else
//             stepper.dec();
//     }

//     control_ring.move_tail_one();
//     return;
// }
// #pragma GCC reset_options