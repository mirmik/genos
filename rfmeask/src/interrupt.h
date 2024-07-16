/**
    @file
*/

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <ralgo/oldheimer/interrupt_args.h>

enum StateCode : uint8_t
{
    Ready,
    NotReady,
    Allarm,
    Disconnect
};

class state_interrupt_args : public heimer::interrupt_args
{
public:
    uint8_t code;
    uint8_t subcode_a;
    uint8_t subcode_b;

    state_interrupt_args(uint8_t code, uint8_t subcode_a, uint8_t subcode_b)
        : code(code), subcode_a(subcode_a), subcode_b(subcode_b)
    {
    }
};

#endif