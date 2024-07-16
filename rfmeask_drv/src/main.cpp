#include <asm/irq.h>
#include <curboard.h>
#include <genos/autom_schedee.h>
#include <genos/ktimer.h>
#include <genos/schedee.h>
#include <igris/dprint.h>
#include <igris/util/cpu_delay.h>
#include <igris/util/stub.h>
#include <main.h>
#include <spl/stm32f4xx_tim.h>
#include <utility/contty2.h>
#include <utility/numcmd.h>
#include <zillot/common/uartring.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/stm32_usart.h>

EventCtr eventctr;

int64_t trigstart_time = 0;
int32_t trigreturn_time = 0;

// Указатели на текущие объекты контроллеров.
MoveController *volatile current_controller_x = nullptr;
MoveController *volatile current_controller_y = nullptr;

//Делегат, вызываемый при приходе внешнего триггера.
igris::delegate<void> trigin_handler(do_nothing);

bool waittrigmode = true;
bool eventctr_mode = false;
bool external_trigger_notify_mode = false;

ZILLOT_UARTRING(serial2_uartring, usart2, 128, 700);
ZILLOT_UARTRING(serial6_uartring, usart6, 128, 700);
genos::zillot_chardev serial2(&serial2_uartring, "serial2");
genos::zillot_chardev serial6(&serial6_uartring, "serial6");

CONTTY2_DECLARE(contty2_cntxt, &serial2);
NUMCMD_CONTEXT(numcmd_cntxt, numcmd_table, &serial6);

// Болванки статических процессов.
genos::autom_schedee contty2_schedee(contty2_automate, &contty2_cntxt);
genos::autom_schedee numcmd_schedee(numcmd_automate, &numcmd_cntxt);

//Функция, завершающая работу при panic.
extern "C" void emergency_halt();
void emergency_halt()
{
    irqs_disable();

    axis_x.stop();
    axis_y.stop();

    //Непрерывное мигание светодиодом.
    while (1)
    {
        stm32_gpio_toggle(GPIOD, 1 << 14);
        igris::delay(100);
    };
};

void controllers_init()
{
    periodic_compare_x.init_state(&axis_x);
    periodic_compare_y.init_state(&axis_y);
}

void TIM4_IRQHandler(void *arg);
void TIM2_IRQHandler(void *arg);
void TIM5_IRQHandler(void *arg);
void EXTI9_5_IRQHandler(void *arg);

void blink(void *arg, genos::ktimer *timer)
{
    board_led.toggle();
    timer->replan();
}

int pulse_loop_count = -1;
void pulse(void *arg, genos::ktimer *timer)
{
    trigout_send_pulse();

    pulse_loop_count--;

    if (pulse_loop_count != 0)
        timer->replan();
}

genos::ktimer blink_timer(blink, nullptr, 500);
genos::ktimer pulse_timer(pulse, nullptr, 1000);

// Формирование смысла байткода контроллера событий.
void eventctr_init()
{
    eventctr.set_handler(
        0, trigout_send_pulse, "send trigout pulse with notify");
    eventctr.set_handler(3,
                         trigout_send_pulse_without_notify,
                         "send trigout pulse without notify");

    eventctr.set_handler(4,
                         trigout_send_pulse_fast_notify,
                         "send trigout pulse with fast notify");

#ifdef BOARD_RL_SYNCRO_V2
    eventctr.set_handler(1, switch_mode_state0_aug, "clear extpin[0]");
    eventctr.set_handler(2, switch_mode_state1_aug, "set extpin[0]");
#endif
}

int external_trigger_number = 0;
void enable_external_trigger_notify_mode(int xy)
{
    external_trigger_number = 0;
    trigin_handler = igris::delegate(
        +[](void *priv) {
            int32_t xy = (int32_t)(intptr_t)priv;
            int32_t value =
                xy == 0 ? axis_x.read_encoder() : axis_y.read_encoder();
            external_trigger_received_send(external_trigger_number, value);
            ++external_trigger_number;
        },
        (void *)(intptr_t)xy);
}

int main()
{
    // Инициализация переферии.
    board_init();
    genos::schedee_manager_init();

    usart2.setup(115200, 'e', 8, 1);
    usart6.setup(115200, 'e', 8, 1);

    board_led.set(1);
    board_led2.set(1);

    dprln();
    dprln("Board initializing ... Success");

    serial2_uartring.begin(&usart2);
    serial6_uartring.begin(&usart6);

    //Инициализация переферии в соответствии с целями текущего проекта.
    project_configure();

    // Инициализация осей.
    axis_init();

    // Инициализация контроллера событий.
    eventctr_init();

    // Инициализация контроллеров.
    controllers_init();

    //Создание процессов.
    blink_timer.plan();

    numcmd_cntxt.debug_mode = false;

    /// Создать процесс, обслуживающий отладочную консоль
    // contty2_schedee.init(contty2_automate, &contty2_cntxt);
    contty2_schedee.start();

    /// Создать процесс, обслуживающий интерфейс взаимодействия с контроллером
    // numcmd_schedee.init(numcmd_automate, &numcmd_cntxt);
    numcmd_schedee.start();

    irqtable_set_handler(TIM4_IRQn, TIM4_IRQHandler, NULL);
    irqtable_set_handler(TIM2_IRQn, TIM2_IRQHandler, NULL);
    irqtable_set_handler(TIM5_IRQn, TIM5_IRQHandler, NULL);
    irqtable_set_handler(EXTI9_5_IRQn, EXTI9_5_IRQHandler, NULL);

    irqs_enable();
    igris::delay(10);

    while (1)
        __schedule__();
};

void __schedule__()
{
    //Системный контекст диспетчера ОС.
    while (1)
    {
        genos::ktimer_manager_step();
        genos::schedee_manager_step();
    }
};

// INTERRUPT

void TIM5_IRQHandler(void *)
{
    //Прерывание по совпадению текущего положения с заданным.
    if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)
    {
        axis_x.trigger_callback();
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
        return;
    };

    BUG();
};

void TIM2_IRQHandler(void *)
{
    //Прерывание по совпадению текущего положения с заданным.
    if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
    {
        axis_y.trigger_callback();
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        return;
    };

    BUG();
};

//Состояния генератора импульсов.
void state0()
{
    stm32_gpio_write(GPIOD, (1 << 2) | (1 << 6), 1);
    stm32_gpio_write(GPIOD, (1 << 4), 0);
    stm32_gpio_write(GPIOB, (1 << 5), 0);
};

void state1()
{
    stm32_gpio_write(GPIOD, (1 << 0) | (1 << 6), 1);
    stm32_gpio_write(GPIOB, (1 << 5), 1);
    stm32_gpio_write(GPIOD, (1 << 2) | (1 << 4), 0);
};

void state2()
{

    stm32_gpio_write(GPIOD, (1 << 4), 1);
    stm32_gpio_write(GPIOB, (1 << 5), 1);
    stm32_gpio_write(GPIOD, (1 << 2) | (1 << 6), 0);
};

void state3()
{
    stm32_gpio_write(GPIOD, (1 << 2) | (1 << 4), 1);
    stm32_gpio_write(GPIOD, (1 << 6), 0);
    stm32_gpio_write(GPIOB, (1 << 5), 0);
};

void TIM4_IRQHandler(void *arg)
{
    // NOTE: Текущая политика использования платы не предполагает
    // использования генератора.

    //Прерывание по переполнению таймера генератора импульсов
    if (TIM4->SR & 0x1)
    {
        Direction dir = axis_x.direction;

        //Если счетчик генератора пуст, состояние неактивное.
        if (axis_x.generator <= 0)
        {
            //Индикатор работы генератора выключить.
            stm32_gpio_write(GPIOD, 1 << 15, 0);
            goto _exit;
        };

        //Включить индикатор работы генератора.
        stm32_gpio_write(GPIOD, 1 << 15, 1);

        //Учет реверса.
        if (axis_x.generator_reverse)
            dir = Axis::dirreverse(dir);

        //Циклическое изменение состояния выходов генератора.
        if (dir == DIRECTION_FORWARD)
        {
            axis_x.pstate = (++axis_x.pstate) & 0x3;
            ++axis_x.count_generated;
        }
        else
        {
            axis_x.pstate = (--axis_x.pstate) & 0x3;
            --axis_x.count_generated;
        }

        switch (axis_x.pstate)
        {
        case 0:
            state0();
            break;
        case 1:
            state1();
            break;
        case 2:
            state2();
            break;
        case 3:
            state3();
            break;
        };
        axis_x.generator--;

    _exit:
        TIM4->SR &= ~0x1;
        return;
    };
};

//Счетчик-делитель входных импульсов.
uint32_t trigin_divider = 1;
uint32_t trig_in_precounter = 0;

//Счетчик входных импульсов.
int32_t trig_in_counter;

void EXTI9_5_IRQHandler(void *)
{
    if (EXTI->PR & (1 << 5))
        BUG();
    if (EXTI->PR & (1 << 6))
        BUG();
    if (EXTI->PR & (1 << 7))
        BUG();
    if (EXTI->PR & (1 << 8))
        BUG();

    //Прерывание по приходу внешнего триггера.
    if (EXTI->PR & (1 << 9))
    {
        EXTI->PR |= (1 << 9);

        //Делитель входных импульсов.
        trig_in_precounter++;
        if (trig_in_precounter == trigin_divider)
        {
            trig_in_counter++;
            trig_in_precounter = 0;

            //Вызов обработчика триггера.
            trigin_handler();
        }
    };
}
