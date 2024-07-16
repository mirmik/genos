#ifndef RALGO_ROBO_STM32_QUADRIC_ENCODER_H
#define RALGO_ROBO_STM32_QUADRIC_ENCODER_H

#include <ralgo/robo/quadric_encoder.h>

namespace robo
{
    class stm32_quadric_encoder : public robo::quadric_encoder<int32_t>
    {
        volatile int32_t *encoder_reg;
        volatile int32_t *compare_reg;

    public:
        stm32_quadric_encoder(volatile int32_t *encoder_reg,
                              volatile int32_t *compare_reg)
            : encoder_reg(encoder_reg), compare_reg(compare_reg)
        {
        }

        int32_t get_value() override
        {
            auto data = *encoder_reg;
            return data;
        }

        void set_compare_value(int32_t cmp)
        {
            *compare_reg = cmp;
        }

        int32_t get_compare_value()
        {
            return *compare_reg;
        }
    };
}

#endif