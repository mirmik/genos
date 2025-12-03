#ifndef GENOS_DRIVERS_TTY_DRIVER_EMULATOR_H
#define GENOS_DRIVERS_TTY_DRIVER_EMULATOR_H

#include <genos/drivers/tty_driver.h>

namespace genos
{
    class tty_driver_emulator : public tty_driver
    {
    public:
        void emulate_receive(const char *data, size_t size)
        {
            for (int i = 0; i < size; ++i)
                emulate_receive(data[i]);
        }

        void emulate_receive(char c)
        {
            receive_newchar(c);
        }
    };
}

#endif