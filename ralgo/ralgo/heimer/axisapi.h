#ifndef RALGO_HEIMER_AXISAPI_H
#define RALGO_HEIMER_AXISAPI_H

#include <igris/datastruct/dlist.h>

namespace heimer
{
    void collect_axis_api();
    int axises_api_command(int argc, char **argv, char *ans, int anslen);
    int igroups_api_command(int argc, char **argv, char *ans, int anslen);
}

#endif
