#ifndef QUADGEN_STEPCTR_H
#define QUADGEN_STEPCTR_H

#include <igris/util/graycode.h>
#include <ralgo/robo/quadgen.h>
#include <ralgo/robo/stepper.h>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/stm32_gpio.h>

#pragma GCC optimize("Ofast")

class quadgen_stm32_2 : public robo::quadgen
{
    zillot::stm32::pin apin = {};
    zillot::stm32::pin bpin = {};

public:
    quadgen_stm32_2(zillot::stm32::pin apin, zillot::stm32::pin bpin)
        : apin(apin), bpin(bpin)
    {
    }

    quadgen_stm32_2() {}
    quadgen_stm32_2(const quadgen_stm32_2 &q) = default;

    __ALWAYS_INLINE
    void apply_state(bool a, bool b)
    {
        apin.set(a);
        bpin.set(b);
    }
};

class quadgen_stm32_4 : public robo::quadgen
{
    zillot::stm32::pin apin0 = {};
    zillot::stm32::pin apin1 = {};
    zillot::stm32::pin bpin0 = {};
    zillot::stm32::pin bpin1 = {};

public:
    quadgen_stm32_4(zillot::stm32::pin apin0,
                    zillot::stm32::pin apin1,
                    zillot::stm32::pin bpin0,
                    zillot::stm32::pin bpin1)
        : apin0(apin0), apin1(apin1), bpin0(bpin0), bpin1(bpin1)
    {
    }

    quadgen_stm32_4() {}
    quadgen_stm32_4(const quadgen_stm32_4 &q) = default;

    __ALWAYS_INLINE
    void apply_state(bool a, bool b)
    {
        apin0.set(a);
        apin1.set(!a);
        bpin0.set(b);
        bpin1.set(!b);
    }
};

#pragma GCC reset_options

#endif