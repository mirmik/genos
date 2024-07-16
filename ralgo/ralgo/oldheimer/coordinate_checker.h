#ifndef RALGO_OLDHEIMER_COORDINATE_CHECKER_H
#define RALGO_OLDHEIMER_COORDINATE_CHECKER_H

#include <igris/container/array_view.h>
//#include <ralgo/geom/zone_check.h>
#include <ralgo/linalg/linalg.h>

namespace heimer
{
    template <class P> class coordinate_checker
    {
        coordinate_checker *next = nullptr;

    public:
        virtual ~coordinate_checker() = default;
        bool check(control_node *dev, P *val, int dim, char *msgbuf)
        {
            if (check_impl(dev, val, dim, msgbuf))
            {
                return true;
            }

            if (next)
            {
                return next->check(dev, val, dim, msgbuf);
            }

            return false;
        }

        virtual bool
        check_impl(control_node *dev, P *val, int dim, char *msgbuf) = 0;
        virtual int command(int, char **)
        {
            return -1;
        }

        void link_next(coordinate_checker *next)
        {
            this->next = next;
        }
    };
}

#endif
