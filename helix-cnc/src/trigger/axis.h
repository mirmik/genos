#ifndef RFMEASURMENT_AXIS_H
#define RFMEASURMENT_AXIS_H

//#include "genos/schedproc/switchScheduler.h"
//#include "genos/kernel/waitserver.h"
//#include "genos/sigslot/delegate.h"

//#include "util/stub.h"
//#include "utilxx/string.h"

#include "direction.h"
#include <igris/event/delegate.h>
#include <ralgo/robo/drivers/stm32_quadric_encoder.h>

#define X_MAX_SPEED 3000
#define X_MOTOR_RESOLUTION 4194304

#define Y_MAX_SPEED 3000
#define Y_MOTOR_RESOLUTION 4194304

void axis_init();

// static float operator*(float f, Direction val) {
//	return f * (float)val;
//}

//Класс драйвера оси.
class Axis
{
public:
    igris::delegate<void> enable_compare = {};
    igris::delegate<void> disable_compare = {};
    igris::delegate<void> trigger_callback = {};
//    volatile bool generator_reverse = 0;
    volatile bool encoder_reverse = 0;
    uint32_t TimerPPS = 0;
    float max_speed = 0;
    float motor_resolution = 0;
    uint32_t enc_numer = 0;
    uint32_t enc_denum = 0;
//    uint32_t gen_numer = 0;
//    uint32_t gen_denum = 0;
    uint32_t spdmul_numer = 0;
    uint32_t spdmul_denum = 0;
    uint16_t speed = 0;

    float enc_multiplier();
    float gen_multiplier();
    float enc_to_gen_multiplier();
    robo::stm32_quadric_encoder * encoder = {};
    volatile Direction direction = {};
    volatile uint8_t pstate = 0;
    
    void set_speed(uint32_t speed);
    void set_direction(Direction dir);
    void set_compare(int32_t cmp);
    int32_t get_compare();
    uint32_t get_speed();
    Direction get_direction();
    bool check_planed_position(int32_t start_encoder, int32_t start_generator,
                               int32_t E);
    int32_t error_pulses(int32_t start_encoder, int32_t start_generator);
  //  bool in_position(int32_t start_encoder, int32_t start_generator, int32_t E);
    //Direction internal_direction();
    static Direction dirreverse(Direction);
    int32_t read_encoder();
    void reset_encoder();
    void stop();
    float spdmulti() { return (float)spdmul_numer / (float)spdmul_denum; }
};

extern Axis axis_x;
extern Axis axis_y;

#endif