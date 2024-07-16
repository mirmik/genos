#include "axis.h"
#include <trigger/timers.h>
//#include <asm/timer.h>
//#include <configure.h>
//#include "genos/io/Serial.h"
//#include "genos/io/SerToSer.h"
//#include "util/math.h"
//#include "hal/gpio.h"

#include <igris/event/delegate.h>
#include <igris/util/stub.h>

//Объекты осей.
Axis axis_x;
Axis axis_y;

//Инициализация осей.
void axis_init()
{
    axis_x.motor_resolution = X_MOTOR_RESOLUTION;

    axis_x.enc_numer = 50;
    axis_x.enc_denum = 1;
    axis_x.spdmul_numer = 1000;
    axis_x.spdmul_denum = 1000;

    axis_x.speed = 200;

    axis_x.max_speed = X_MAX_SPEED;

    //axis_x.encoder_reg = &TIM5->CNT;
    //axis_x.genfreq_reg = &TIM4->ARR;
    //axis_x.compare_reg = &TIM5->CCR3;

    axis_x.TimerPPS = 8400000;

  //  axis_x.generator = 0;
    axis_x.direction = DIRECTION_FORWARD;
    axis_x.encoder_reverse = true;

    axis_x.enable_compare = tim5_compare_enable;
    axis_x.disable_compare = tim5_compare_disable;
    axis_x.trigger_callback = do_nothing;

    //axis_x.update_speed();

    axis_y.motor_resolution = X_MOTOR_RESOLUTION;

    axis_y.enc_numer = 50;
    axis_y.enc_denum = 1;
    axis_y.spdmul_numer = 1000;
    axis_y.spdmul_denum = 1000;

    axis_y.speed = 200;

    axis_y.max_speed = X_MAX_SPEED;

    //axis_y.encoder_reg = &TIM2->CNT;
    //axis_y.genfreq_reg = &TIM4->ARR;
    //axis_y.compare_reg = &TIM2->CCR3;

    axis_y.TimerPPS = 8400000;

//    axis_y.generator = 0;
    axis_y.direction = DIRECTION_FORWARD;
    axis_y.encoder_reverse = true;

    axis_y.enable_compare = tim2_compare_enable;
    axis_y.disable_compare = tim2_compare_disable;
    axis_y.trigger_callback = do_nothing;

    //axis_y.update_speed();
}

int32_t Axis::read_encoder()
{
    int32_t ret = encoder->get_value();
    return encoder_reverse ? -ret : ret;
};

void Axis::set_direction(Direction dir) { direction = dir; };

Direction Axis::get_direction() { return direction; };

//Установить точку для сравнения.
void Axis::set_compare(int32_t cmp)
{
    encoder->set_compare_value(encoder_reverse ? -cmp : cmp);
};

//Запросить позицию сравнения.
int32_t Axis::get_compare()
{
    int32_t val = encoder->get_compare_value();
    return encoder_reverse ? -val : val;
};

void Axis::reset_encoder() 
{
    encoder->reset_value();
}

/*Direction Axis::internal_direction()
{
    Direction dir = direction;
    if (generator_reverse)
        dir = dirreverse(dir);
    return dir;
};*/

float Axis::enc_multiplier() { return (float)enc_numer / (float)enc_denum; };

Direction Axis::dirreverse(Direction dir)
{
    return dir == DIRECTION_FORWARD ? DIRECTION_BACKWARD : DIRECTION_FORWARD;
};