#ifndef RALGO_ROBO_QUADRIC_ENCODER_H
#define RALGO_ROBO_QUADRIC_ENCODER_H

namespace robo
{
    template <class T> class quadric_encoder
    {
    public:
        virtual T get_value() = 0;
        virtual void reset_value() = 0;
    };
}

#endif