#include <ralgo/heimer/scalar_signal.h>
#include <ralgo/heimer/sigtypes.h>
#include <stdio.h>

int heimer::scalar_signal::info(char *data, int maxsize)
{
    snprintf(data, maxsize, "(value:%f)\r\n", value);
    return 0;
}

heimer::scalar_signal::scalar_signal(const char *name)
    : signal_head(name, SIGNAL_TYPE_SCALAR)
{
}
