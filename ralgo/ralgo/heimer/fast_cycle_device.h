#ifndef RALGO_HEIMER_FAST_CYCLE_DEVICE_H
#define RALGO_HEIMER_FAST_CYCLE_DEVICE_H

#include <igris/datastruct/dlist.h>

namespace heimer
{
    extern dlist_head fast_cycle_list;

    class fast_cycle_device
    {
    public:
        dlist_head fast_cycle_list_lnk = DLIST_HEAD_INIT(fast_cycle_list_lnk);

    public:
        fast_cycle_device() {}

        void bind()
        {
            dlist_move(&fast_cycle_list_lnk, &fast_cycle_list);
        }

        virtual int fast_cycle_serve() = 0;
        virtual ~fast_cycle_device() = default;
    };

    double fast_cycle_frequence();
}

#endif
