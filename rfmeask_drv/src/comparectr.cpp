#include "comparectr.h"
#include <curboard.h>
#include <functions.h>
#include <genos/displace.h>
#include <genos/schedee_api.h>
#include <igris/time/systime.h>
#include <igris/util/cpu_delay.h>
#include <igris/util/numconvert.h>
#include <igris/util/stub.h>
#include <main.h>
#include <string.h>
#include <zillot/common/uartring.h>

//Объекты контроллеров сканирования.
PeriodicCompareController periodic_compare_x;
PeriodicCompareController periodic_compare_y;

extern int disable_trigin_mode;
extern volatile bool notify_on_answer_pulse;

//По приходу триггера снять флаг ожидания и отправить метку.
void PeriodicCompareController::simpleMode_triginHandler()
{
    count_in++;

    trigwait = false;
    trigwait_hook.invoke_and_reset();

    if (notify_on_answer_pulse)
    {
        notify_on_answer_pulse = false;
        last_scan_information = axis->read_encoder();
        scan_command_send(count_in);
    }
}

//Генерация импульса trigout.
// Обработчик триггера совпадения по умолчанию.
void PeriodicCompareController::simpleMode_trigoutHandler()
{
    //В случае генерации нового сигнала до прихода эхо от старого сгенерировать
    //ошибку.
    if (waittrigmode)
    {
        if (trigwait == true)
        {
            if (trigger_error_counter > 3)
            {
                scan_error_send();
                _removeflag =
                    true; // removeflag используется, из-за некоректной
                          // обработки выхода из прерывания.
                return;
            }

            // Источник нестабильности.
            // Вероятно, стоит выкинуть этот код.
            trigger_error_counter++;
            simpleMode_triginHandler();
        };

        //Установить флаг ожидания.
        trigwait = true;

        //Сгенерировать импульс.
        pulse_generate();
    }
    else
    {
        pulse_generate();
        scan_command_send(waited_point_counter);
    }

    set_next_point_to_compare_or_disable_compairing_if_last();
};

// Обработчик триггера совпадения для режима eventctr.
void PeriodicCompareController::eventctrTrigoutHandler()
{
    if (((trigwait == true) && (!disable_trigin_mode)))
    {
        scan_error_send();
        _removeflag = true; // removeflag используется, из-за некоректной
                            // обработки выхода из прерывания.
        return;
    };

    // Вызвать обработчик согласно байткоду программы сканирования.
    eventctr.do_next_event();
    set_next_point_to_compare_or_disable_compairing_if_last();
}

void PeriodicCompareController::
    set_next_point_to_compare_or_disable_compairing_if_last()
{
    //Если точка не последняя, включить ожидание следующей.
    if (waited_point_counter < incpos_points)
    {
        int32_t compare = next_compare_point();
        axis->set_compare(compare);
    }
    //Иначе, отключить сравнение.
    else
        axis->disable_compare();
}

int32_t PeriodicCompareController::next_compare_point()
{
    int32_t result;

    if (!use_distance_table)
    {
        result =
            start_enc +
            ((incpos_scan_strt_zone + 
                (waited_point_counter)*compare_interval) /
            axis->enc_multiplier() * axis->direction);
    }

    else
    {
        result =
            start_enc + 
            ((incpos_scan_strt_zone + disttbl.get_next_point()) /
            axis->enc_multiplier() * axis->direction);
    }

    ++waited_point_counter;
    return result;
}

//Процесс контроллера сканирования.
int PeriodicCompareController::exec()
{
    _runstate = true;
    count_in = 0;
    waited_point_counter = 0;
    trigger_error_counter = 0;

    init_new_scan();

    //Если отсутствует зона разгона, то первую точку начинаем снимать сразу же
    //Иначе расчитываем первую точку.
    if (incpos_scan_strt_zone == 0)
    {
        // Первая точка как бы уже расчитана и найдена,
        // поэтому устанавливаем сразу индекс 1.
        waited_point_counter = 1;
        axis->trigger_callback();
    }
    else
    {
        axis->set_compare(next_compare_point());
    }

    axis->enable_compare();

    //Логика контроля движения.
    while (incpos_points != counter_what_we_want())
    {
        if (_removeflag)
            break;
        __displace__();
    };

    genos::current_schedee_msleep(200);
    remove_impl();
    return 0;
};

int64_t PeriodicCompareController::counter_what_we_want()
{
    if (waittrigmode)
        return count_in;
    else
        return waited_point_counter - 1;
}

void PeriodicCompareController::set_start_position()
{
    start_enc = axis->read_encoder();
};

// extern MoveController* current_controller;
extern igris::delegate<void> trigin_handler;
extern uint32_t trig_in_precounter;

void PeriodicCompareController::remove_impl()
{
    axis->disable_compare();

    if (this == current_controller_x)
        current_controller_x = 0;
    if (this == current_controller_y)
        current_controller_y = 0;

    scan_stop_send(); //Сообщение о прекращении работы контроллера.

    if (!external_trigger_notify_mode)
        trigin_handler = do_nothing;
    _runstate = false;
    _removeflag = false;
};

void PeriodicCompareController::init_new_scan()
{
    if (!external_trigger_notify_mode)
        trigin_handler = do_nothing;
    axis->disable_compare();

    _removeflag = false;
    trigwait = false;
    trig_in_precounter = 0;   //Сброс предсчетчика.
    waited_point_counter = 0; //Счетчик точек сравнения.
    count_in = 0; // счетчик входных триггеров

    notify_on_answer_pulse = false;

    eventctr.counter = 0;
    eventctr.cycle_count = 0;

    //Установка делегата сравнения положения.
    if (!eventctr_mode)
    {
        axis->trigger_callback = igris::make_delegate(
            &PeriodicCompareController::simpleMode_trigoutHandler, this);
    }
    else // if eventctr_mode
    {
        axis->trigger_callback = igris::make_delegate(
            &PeriodicCompareController::eventctrTrigoutHandler, this);

        incpos_points = (eventctr.cmdtable_size_prefix +
                         eventctr.cmdtable_size_cycle * eventctr.cycle_total +
                         eventctr.cmdtable_size_postfix);
    }

    if (waittrigmode)
    {
        //Установка делегата входного триггер.
        trigin_handler = igris::make_delegate(
            &PeriodicCompareController::simpleMode_triginHandler, this);

        //Делегат генерации пульса.
        pulse_generate = trigout_send_pulse;
    }

    //Сброс генератора.
    axis->set_generator(0);

    //Установка направления.
    axis->set_direction(incpos_direction);

    set_start_position(); //Запомнить начальное положение.

    //Расчет интервала следования точек сканирования.
    compare_interval = ((double)(incpos_move - incpos_scan_stop_zone -
                                 incpos_scan_strt_zone)) /
                       (double)(incpos_points - 1);
}

void PeriodicCompareController::clear_compare_table()
{
    disttbl.clear();
}

void PeriodicCompareController::add_compare_table_point(int32_t enc_pos)
{
    disttbl.add_point(enc_pos);
}

void PeriodicCompareController::enable_compare_table(bool en)
{
    use_distance_table = en;
    board_led2.set(!en); // enable/disable led. inversed.
}

void PeriodicCompareController::reset()
{
    // todo : drop settings to default parameters
}