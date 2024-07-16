#ifndef POINT_INFORMATION_BUFFER
#define POINT_INFORMATION_BUFFER

#include <igris/container/ring.h>

class PointInformationBuffer
{
public:
    igris::ring<double> points = igris::ring<double>(50000);

    void push(double &pi)
    {
        points.push(pi);
    }

    double last()
    {
        return points.last();
    }

    bool empty()
    {
        return points.empty();
    }
};

#endif