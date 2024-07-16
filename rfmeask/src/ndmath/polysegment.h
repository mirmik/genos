#ifndef RABBIT_MESHMATH_POLYSEGMENT_H
#define RABBIT_MESHMATH_POLYSEGMENT_H

#include <ndmath/point.h>
#include <ndmath/segment.h>
#include <vector>

namespace ndmath
{
    class polysegment
    {
        std::vector<ndmath::point> data={};
        bool closed = false;

    public:
        polysegment() = default;
        polysegment(const polysegment &) = default;
        polysegment(polysegment &&) = default;
        polysegment &operator=(const polysegment &) = default;
        polysegment &operator=(polysegment &&) = default;

        void add_point(const ndmath::point &point) { data.push_back(point); }
        std::vector<ndmath::point> &points() { return data; }
        size_t npoints() const { return data.size(); }
        size_t nsegments() const { return data.size() - 1; }

        std::vector<ndmath::segment> segments()
        {
            std::vector<ndmath::segment> ret;
            for (size_t i = 0; i < nsegments(); i++)
            {
                ret.emplace_back(data[i], data[i + 1]);
            }

            if (closed)
            {
                ret.emplace_back(data[npoints() - 1], data[0]);
            }

            return ret;
        }
    };
}

#endif