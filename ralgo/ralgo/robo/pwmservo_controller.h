#ifndef RALGO_ROBO_PWMSERVO_CONTROLLER_H
#define RALGO_ROBO_PWMSERVO_CONTROLLER_H

#include <ralgo/robo/iposvel.h>

namespace robo
{
    class pwmservo_controller : public i_position_driver
    {
        void set_position(double pos) override;
    };
}

#endif
