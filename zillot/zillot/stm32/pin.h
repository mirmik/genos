#ifndef ZILLOT_STM32_GPIO_H
#define ZILLOT_STM32_GPIO_H

#include <stdint.h>
#include <stm32xxxx.h>
#include <zillot/common/abstract_pin.h>
#include <zillot/common/gpio.h>
#include <zillot/gpiodefs.h>
#include <zillot/stm32/stm32_gpio.h>

namespace zillot
{
    namespace stm32
    {
        class pin : public zillot::abstract_pin
        {
        public:
            GPIO_TypeDef *regs;
            int mask;

        public:
            pin() = default;
            pin(GPIO_TypeDef *gpio, int mask) : regs(gpio), mask(mask) {}
            pin(const pin &oth) = default;
            pin &operator=(const pin &oth) = default;

            void write(int val) const override
            {
                stm32_gpio_write(regs, mask, val);
            }

            int read() const override
            {
                return stm32_gpio_read(regs, mask);
            }

            void toggle() const override
            {
                return stm32_gpio_toggle(regs, mask);
            }

            void pull_up(int lvl) const override
            {
                return stm32_gpio_pull_up(regs, mask, lvl);
            }

            int setup(uint32_t mode) const override
            {
                return stm32_gpio_setup(regs, mask, mode);
            }

            void setup_alternate(int afno) const
            {
                stm32_gpio_set_alternate(regs, mask, afno);
            }
        };
    }
}

#endif