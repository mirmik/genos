#ifndef RALGO_TRAJECTORY_H
#define RALGO_TRAJECTORY_H

#include <new>
#include <ralgo/disctime.h>
#include <ralgo/heimer/heimer_types.h>
#include <ralgo/trajectory/tsdeform.h>

class trajectory
{
public:
    int dim = 0;

public:
    virtual int
    attime(disctime_t timestamp, position_t *outpos, position_t *outspd) = 0;
    void init(int dim);
    virtual ~trajectory() = default;
};

#endif
