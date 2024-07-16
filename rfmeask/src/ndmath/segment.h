#ifndef RABBIT_NDSEGMENT_H
#define RABBIT_NDSEGMENT_H

#include <ralgo/linalg/vecops.h>
#include <ralgo/linalg/vector.h>

namespace ndmath
{
    class segment
    {
    public:
        ndmath::point apnt;
        ndmath::point bpnt;

        segment(const ndmath::point &apnt, const ndmath::point &bpnt)
            : apnt(apnt), bpnt(bpnt)
        {
        }

        double length() const { return ralgo::vecops::length(bpnt - apnt); }
    };
}

#endif