#include <stddef.h>
#include <string.h>

#include <igris/util/member.h>

#include <ralgo/heimer/convex_zone_approval.h>
#include <ralgo/lp/point_in_hexagon.h>

int heimer::convex_zone_approval::check(int dim,
                                        position_t *strt,
                                        position_t *fini)
{
    (void)strt;

    int in = point_in_hexagon_d(table, dim, points_total, fini);
    return in;
}

void heimer::convex_zone_approval::init(int dim)
{
    this->dim = dim;
    table = NULL;
    points_total = 0;
    points_capacity = 0;
}

int heimer::convex_zone_approval::room()
{
    return points_capacity - points_total;
}

void heimer::convex_zone_approval::bind_table(position_t *table,
                                              int cap,
                                              int size)
{
    this->table = table;
    this->points_capacity = cap;
    this->points_total = size;
}

void heimer::convex_zone_approval::extend(position_t *pnt, int size)
{
    int _room = room();
    int toload = MIN(_room, size);

    memcpy(table + points_total * dim, pnt, toload * dim * sizeof(position_t));

    points_total += toload;
}
