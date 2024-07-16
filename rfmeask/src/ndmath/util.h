#ifndef NDMATH_UTIL_H
#define NDMATH_UTIL_H

#include <ndmath/cartesian_correction.h>
#include <nos/trent/trent.h>

namespace one_axis_cartesian_correction
{
    ndmath::cartesian_correction trent_to_correction(const nos::trent &tr);
    ndmath::cartesian_correction
    arrays_to_correction(std::vector<double> points,
                         std::vector<double> deltas);

    nos::trent correction_to_trent(const ndmath::cartesian_correction &);
    std::pair<std::vector<double>, std::vector<double>>
    correction_to_arrays(const ndmath::cartesian_correction &);

    ndmath::cartesian_correction correction_from_csv_file(std::string fname);
}

namespace multi_axes_cartesian_correction
{
    ndmath::cartesian_correction trent_to_correction(const nos::trent &tr);
    ndmath::cartesian_correction
    arrays_to_correction(std::vector<double> points,
                         std::vector<double> deltas);

    nos::trent correction_to_trent(const ndmath::cartesian_correction &);
    std::pair<std::vector<double>, std::vector<double>>
    correction_to_arrays(const ndmath::cartesian_correction &);
}

#endif