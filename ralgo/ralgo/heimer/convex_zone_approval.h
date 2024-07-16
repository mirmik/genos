#ifndef RALGO_HEIMER_CONVEX_ZONE_APPROVAL_H
#define RALGO_HEIMER_CONVEX_ZONE_APPROVAL_H

#include <new>
#include <ralgo/heimer/axisctr_approval.h>

namespace heimer
{
    class convex_zone_approval : public axisctr_approval
    {
    public:
        double *table = nullptr;
        int dim = 0;

        int points_total = 0;
        int points_capacity = 0;

    public:
        void init(int dim);
        int check(int dim, position_t *strt, position_t *fini) override;
        int room();
        void bind_table(position_t *table, int cap, int size);
        void extend(position_t *pnt, int size);
    };
}

#endif
