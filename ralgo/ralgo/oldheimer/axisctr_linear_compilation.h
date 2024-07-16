#ifndef HEIMER_AXISCTR_LINEAR_COMPILATION_H
#define HEIMER_AXISCTR_LINEAR_COMPILATION_H

#include <ralgo/oldheimer/axisctr.h>
#include <ralgo/oldheimer/linintctr.h>

namespace heimer
{
    template <class P, class V>
    class axisctr_linear_compilation : public axisctr<P, V>
    {
        linintctr<P, V, 2> *ctr1;
        linintctr<P, V, 2> *ctr2;

        // double koeff1;
        // double koeff2;

    public:
        axisctr_linear_compilation(const char *name) : axisctr<P, V>(name) {}

        axisctr_linear_compilation(const char *name,
                                   linintctr<P, V, 2> *_ctr1,
                                   linintctr<P, V, 2> *_ctr2)
            : axisctr_linear_compilation(name)
        {
            ctr1 = _ctr1;
            ctr2 = _ctr2;
        }

        int incmove_unsafe(P dist) override
        {
            abort();
            return 0;
        }

        int absmove_unsafe(P coord) override
        {
            float pos1[2]{coord, 0};
            float pos2[2]{coord, 0};

            ctr1->absmove(pos1, true);
            ctr2->absmove(pos2, true);
            return 0;
        }
    };
}

#endif
