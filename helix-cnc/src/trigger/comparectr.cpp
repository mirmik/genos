#include "comparectr.h"
#include <cstring>
#include <genos/displace.h>
#include <genos/schedee_api.h>
#include <igris/time/systime.h>
#include <igris/util/cpu_delay.h>
#include <igris/util/numconvert.h>
#include <igris/util/stub.h>
#include <main.h>
#include <trigger/global.h>
#include <zillot/common/uartring.h>

//Объекты контроллеров сканирования.
PeriodicCompareController periodic_compare_x;
PeriodicCompareController periodic_compare_y;

//По приходу триггера снять флаг ожидания и отправить метку.
void PeriodicCompareController::triginHandler()
{
    count_in++;

    if (bitrigmode)
    {
        if (trigin_armed)
        {
            bitrigmode_second_state_handler();
            cpu_delay(100);
            pulse_generate();
            trigin_armed = false;
        }

        else
        {
            trigwait = false;
            bitrigmode_first_state_handler();
        }
    }

    else
    {
        trigwait = false;
    }

    trigwait_hook.invoke_and_reset();

    if (notify_on_answer_pulse)
    {
        notify_on_answer_pulse = false;
        scan_command_send();
    }
}

//Генерация импульса trigout.
// Обработчик триггера совпадения по умолчанию.
void PeriodicCompareController::trigoutHandler()
{
    if (bitrigmode)
    {
        trigin_armed = true;
    }

    //В случае генерации нового сигнала до прихода эхо от старого сгенерировать
    //ошибку.
    if (waittrigmode)
    {
        if (trigwait == true)
        {
            if (trigger_error_counter > 3)
            {
                scan_error_send();
                removeflag = true; // removeflag используется, из-за некоректной
                                   // обработки выхода из прерывания.
                return;
            }
            else
            {
                trigger_error_counter++;
                triginHandler();
            }
        };

        //Установить флаг ожидания.
        trigwait = true;

        //Сгенерировать импульс.
        pulse_generate();
    }
    else
    {
        pulse_generate();
        scan_command_send();
    }

    //Расчитать точку следующего тригера.
    int32_t compare =
        start_enc + (incpos_scan_strt_zone + (++count) * compare_interval) /
                        axis->enc_multiplier() * axis->direction;

    //Если точка не последняя, включить ожидание следующей.
    if (count < incpos_points)
        axis->set_compare(compare);
    //Иначе, отключить сравнение.
    else
        axis->disable_compare();
};

// Обработчик триггера совпадения для режима eventctr.
void PeriodicCompareController::eventctrTrigoutHandler()
{
    if (((trigwait == true) && (!disable_trigin_mode)))
    {
        scan_error_send();
        removeflag = true; // removeflag используется, из-за некоректной
                           // обработки выхода из прерывания.
        return;
    };
    triggers_points++;

    // Вызвать обработчик согласно байткоду программы сканирования.
    eventctr.do_next_event();

    //Расчитать точку следующего тригера.
    int32_t compare =
        start_enc + ((incpos_scan_strt_zone + (++count) * compare_interval) /
                     axis->enc_multiplier() * axis->direction);

    //Если точка не последняя, включить ожидание следующей.
    if (count < incpos_points)
        axis->set_compare(compare);

    //Иначе, отключить сравнение.
    else
        axis->disable_compare();
}

//Процесс контроллера сканирования.
/*void *PeriodicCompareController::exec()
{
    //_runstate = true;
    triggers_points = 0;

    trigger_error_counter = 0;
    init_new_scan();
    bitrigmode_first_state_handler();

    //Если отсутствует зона разгона, то первую точку начинаем снимать сразу же
    if (incpos_scan_strt_zone == 0)
    {
        axis->trigger_callback();
    }

    //Иначе расчитываем первую точку.
    else
    {
        axis->set_compare(start_enc + incpos_scan_strt_zone /
                                          axis->enc_multiplier() *
                                          axis->direction);
    }

    axis->enable_compare();

    //Логика контроля движения.
    while (incpos_points != count)
    {
        if (removeflag)
        {
            remove();
        }
        __displace__();
    };

    current_schedee_msleep(200);
    remove();
    return NULL;
};*/

void PeriodicCompareController::start()
{
    triggers_points = 0;
    trigger_error_counter = 0;
    init_new_scan();
    bitrigmode_first_state_handler();

    //Если отсутствует зона разгона, то первую точку начинаем снимать сразу же
    if (incpos_scan_strt_zone == 0)
    {
        axis->trigger_callback();
    }

    //Иначе расчитываем первую точку.
    else
    {
        axis->set_compare(start_enc + incpos_scan_strt_zone /
                                          axis->enc_multiplier() *
                                          axis->direction);
    }

    _runned = true;
    axis->enable_compare();
}

void PeriodicCompareController::finish()
{
    bitrigmode = false;
    axis->disable_compare();
    scan_stop_send(); //Сообщение о прекращении работы контроллера.
    trigin_handler = {};
    _runned = false;
}

void PeriodicCompareController::set_start_position()
{
    start_enc = axis->read_encoder();
};

void PeriodicCompareController::init_new_scan()
{
    trigin_handler = do_nothing;
    axis->disable_compare();

    removeflag = false;
    trigwait = false;
    trig_in_precounter = 0; //Сброс предсчетчика.

    notify_on_answer_pulse = false;

    eventctr.counter = 0;
    eventctr.cycle_count = 0;

    //Установка делегата сравнения положения.
    if (!eventctr_mode)
    {
        axis->trigger_callback = igris::make_delegate(
            &PeriodicCompareController::trigoutHandler, this);
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
            &PeriodicCompareController::triginHandler, this);

        //Делегат генерации пульса.
        pulse_generate = igris::make_delegate(trigout_send_pulse);
    }

    //Установка направления.
    axis->set_direction(incpos_direction);

    count = 0; //Счетчик точек сравнения.
    count_in = 0;
    set_start_position(); //Запомнить начальное положение.

    //Расчет интервала следования точек сканирования.
    compare_interval =
        ((float)(incpos_move - incpos_scan_stop_zone - incpos_scan_strt_zone)) /
        (float)(incpos_points - 1);
};

void PeriodicCompareController::scan_command_send() {}

void PeriodicCompareController::scan_stop_send() {}

void PeriodicCompareController::scan_error_send() {}

bool PeriodicCompareController::runned()
{
    return _runned;
}