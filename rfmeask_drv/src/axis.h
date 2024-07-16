#ifndef RFMEASURMENT_AXIS_H
#define RFMEASURMENT_AXIS_H

#include "direction.h"
#include <igris/event/delegate.h>

#define X_MAX_SPEED 3000
#define X_MOTOR_RESOLUTION 4194304

#define Y_MAX_SPEED 3000
#define Y_MOTOR_RESOLUTION 4194304

void axis_init();

//Класс драйвера оси.
class Axis
{
public:
    float max_speed = 0;
    float motor_resolution = 0;
    uint32_t TimerPPS = 0;

    igris::delegate<void> enable_compare = {};
    igris::delegate<void> disable_compare = {};

    volatile bool generator_reverse = false;
    volatile bool encoder_reverse = false;

    uint32_t enc_numer = 0;
    uint32_t enc_denum = 0;

    uint32_t gen_numer = 0;
    uint32_t gen_denum = 0;

    uint32_t spdmul_numer = 0;
    uint32_t spdmul_denum = 0;

    uint16_t speed = 0;

    volatile uint32_t *encoder_reg = nullptr;
    volatile uint32_t *genfreq_reg = nullptr;
    volatile uint32_t *compare_reg = nullptr;
    volatile uint32_t generator = 0;
    volatile int32_t count_generated = 0;
    volatile Direction direction = {};
    volatile uint8_t pstate = 0;
    igris::delegate<void> trigger_callback = {};

public:
    float enc_multiplier();
    float gen_multiplier();
    float enc_to_gen_multiplier();
    float gen_to_enc_multiplier();

    void update_speed();
    void set_speed(uint32_t speed);
    void set_generator(uint64_t pulse);
    void set_direction(Direction dir);
    void set_compare(int32_t cmp);

    int32_t get_compare();
    uint32_t get_speed();
    Direction get_direction();

    bool check_planed_position(int32_t start_encoder,
                               int32_t start_generator,
                               int32_t E);
    int32_t error_pulses(int32_t start_encoder, int32_t start_generator);
    bool in_position(int32_t start_encoder, int32_t start_generator, int32_t E);

    Direction internal_direction();
    static Direction dirreverse(Direction);

    int32_t read_encoder();
    int32_t read_generator();
    int32_t read_count_generator();

    void stop();

    float spdmulti()
    {
        return (float)spdmul_numer / (float)spdmul_denum;
    }
};

extern Axis axis_x;
extern Axis axis_y;

#endif