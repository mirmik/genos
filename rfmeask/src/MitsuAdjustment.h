/**
    @file
*/

#ifndef MITSUBISHIBASIC_H
#define MITSUBISHIBASIC_H

#include <include_helper.h>
#include <nos/trent/trent.h>
#include <stdlib.h>

struct MitsuAdjustment
{
    int type = 0;
    int table_number = 0;
    double inertia_rate = 0;

    MitsuAdjustment();
    MitsuAdjustment(const nos::trent &tr);
    MitsuAdjustment(const MitsuAdjustment &oth);

    MitsuAdjustment &operator=(const MitsuAdjustment &oth);
    nos::expected<size_t, nos::output_error> print_to(nos::ostream &o) const;
};

#endif // MITSUBISHIBASIC_H
