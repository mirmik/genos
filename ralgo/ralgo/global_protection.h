#ifndef RALGO_PROTECT_H
#define RALGO_PROTECT_H

namespace ralgo
{
    extern volatile bool global_protection;

    void set_global_protection(bool en);
    int set_global_protection_command(int argc, char **argv);
}

#endif
