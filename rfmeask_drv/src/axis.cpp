#include "axis.h"
#include <cmath>
#include <configure.h>
#include <igris/event/delegate.h>
#include <igris/util/stub.h>
#include <zillot/stm32/stm32_timer.h>

//Объекты осей.
Axis axis_x;
Axis axis_y;

//Инициализация осей.
void axis_init()
{
    axis_x.enc_numer = 50;
    axis_x.enc_denum = 1;
    axis_x.gen_numer = 1000;
    axis_x.gen_denum = 1;
    axis_x.spdmul_numer = 1000;
    axis_x.spdmul_denum = 1000;

    axis_x.speed = 200;

    axis_x.max_speed = X_MAX_SPEED;

    axis_x.encoder_reg = &TIM5->CNT;
    axis_x.genfreq_reg = &TIM4->ARR;
    axis_x.compare_reg = &TIM5->CCR3;

    axis_x.TimerPPS = 8400000;

    axis_x.generator = 0;
    axis_x.direction = DIRECTION_FORWARD;
    axis_x.generator_reverse = true;
    axis_x.encoder_reverse = true;

    axis_x.enable_compare = tim5_compare_enable;
    axis_x.disable_compare = tim5_compare_disable;
    axis_x.trigger_callback = do_nothing;

    axis_x.update_speed();

    axis_x.motor_resolution = X_MOTOR_RESOLUTION;
    axis_y.motor_resolution = X_MOTOR_RESOLUTION;

    axis_y.enc_numer = 50;
    axis_y.enc_denum = 1;
    axis_y.gen_numer = 1000;
    axis_y.gen_denum = 1;
    axis_y.spdmul_numer = 1000;
    axis_y.spdmul_denum = 1000;

    axis_y.speed = 200;

    axis_y.max_speed = X_MAX_SPEED;

    axis_y.encoder_reg = &TIM2->CNT;
    axis_y.genfreq_reg = &TIM4->ARR;
    axis_y.compare_reg = &TIM2->CCR3;

    axis_y.TimerPPS = 8400000;

    axis_y.generator = 0;
    axis_y.direction = DIRECTION_FORWARD;
    axis_y.generator_reverse = true;
    axis_y.encoder_reverse = true;

    axis_y.enable_compare = tim2_compare_enable;
    axis_y.disable_compare = tim2_compare_disable;
    axis_y.trigger_callback = do_nothing;

    axis_y.update_speed();
}

//Проверка позиции на принадлежность интервалу (в движении! Поэтому она такая
//странная).
bool Axis::check_planed_position(int32_t start_encoder,
                                 int32_t start_generator,
                                 int32_t E)
{
    int32_t planed =
        (read_count_generator() - start_generator) * gen_to_enc_multiplier();
    int32_t real = read_encoder() - start_encoder;
    int32_t diff = real - planed;
    return -E < diff && diff > E;
}

//Находится ли изделие в целевой позиции?
bool Axis::in_position(int32_t start_encoder,
                       int32_t start_generator,
                       int32_t E)
{
    return generator == 0 &&
           std::abs(error_pulses(start_encoder, start_generator)) < E;
}

int32_t Axis::error_pulses(int32_t start_encoder, int32_t start_generator)
{
    int32_t planed =
        (read_count_generator() - start_generator) * gen_to_enc_multiplier();
    int32_t real = read_encoder() - start_encoder;
    return planed - real;
}

void Axis::set_speed(uint32_t _speed)
{
    speed = _speed;
    update_speed();
}

void Axis::update_speed()
{
    float speed_rev_min;
    float speed_rev_sec;
    float pps;

    speed_rev_min = speed;

    if (speed_rev_min > max_speed)
        speed_rev_min = max_speed;

    speed_rev_sec = speed_rev_min / 60.0;
    pps = speed_rev_sec * motor_resolution / gen_multiplier() * spdmulti();

    int res = (float)TimerPPS / pps;
    if (res < 15)
        res = 15;
    *genfreq_reg = res;
}

uint32_t Axis::get_speed()
{
    //	float speed_rev_min;
    float speed_rev_sec;
    float pps;

    pps = (float)TimerPPS / *genfreq_reg;
    speed_rev_sec = pps / motor_resolution * gen_multiplier();
    return speed_rev_sec * 60.0;
};

void Axis::set_generator(uint64_t arg)
{
    generator = arg;
};

int32_t Axis::read_encoder()
{
    int32_t ret = *encoder_reg;
    return encoder_reverse ? -ret : ret;
};

int32_t Axis::read_generator()
{
    return generator;
};

//Количество сгенирированных импульсов с учетом глобального реверса направления.
int32_t Axis::read_count_generator()
{
    int32_t ret = count_generated;
    return generator_reverse ? -ret : ret;
};

void Axis::set_direction(Direction dir)
{
    direction = dir;
};

Direction Axis::get_direction()
{
    return direction;
};

//Установить точку для сравнения.
void Axis::set_compare(int32_t cmp)
{
    *compare_reg = encoder_reverse ? -cmp : cmp;
};

//Запросить позицию сравнения.
int32_t Axis::get_compare()
{
    return encoder_reverse ? -*compare_reg : *compare_reg;
};

Direction Axis::internal_direction()
{
    Direction dir = direction;
    if (generator_reverse)
        dir = dirreverse(dir);
    return dir;
};

void Axis::stop()
{
    //Не генерировать импульсы.
    generator = 0;
};

float Axis::enc_multiplier()
{
    return (float)enc_numer / (float)enc_denum;
};

float Axis::gen_multiplier()
{
    return (float)gen_numer / (float)gen_denum;
};

float Axis::gen_to_enc_multiplier()
{
    return gen_multiplier() / enc_multiplier();
};

float Axis::enc_to_gen_multiplier()
{
    return enc_multiplier() / gen_multiplier();
};

Direction Axis::dirreverse(Direction dir)
{
    return dir == DIRECTION_FORWARD ? DIRECTION_BACKWARD : DIRECTION_FORWARD;
};