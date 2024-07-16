#ifndef RALGO_HEIMER_TYPES_H
#define RALGO_HEIMER_TYPES_H

#include <igris/math.h>

typedef double position_t;
typedef double velocity_t;
typedef double acceleration_t;

__BEGIN_DECLS

static inline velocity_t heimvel(double val)
{
    return val;
}
static inline position_t heimpos(double val)
{
    return val;
}
static inline position_t heimdist(double val)
{
    return val;
}
static inline acceleration_t heimacc(double val)
{
    return val;
}
static inline position_t heimdeg(double val)
{
    return val / 180. * M_PI;
}

static inline double heimpos_cos(position_t val)
{
    return cos(val);
}
static inline double heimpos_sin(position_t val)
{
    return sin(val);
}

static inline double heimvel_restore(position_t val)
{
    return val;
}
static inline double heimpos_restore(velocity_t val)
{
    return val;
}
static inline double heimdist_restore(velocity_t val)
{
    return val;
}
static inline double heimacc_restore(acceleration_t val)
{
    return val;
}

__END_DECLS

#endif
