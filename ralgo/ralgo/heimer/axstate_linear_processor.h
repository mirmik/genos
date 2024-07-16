/** @file */

#ifndef RALGO_HEIMER_AXSTATE_LINEAR_PROCESSOR_H
#define RALGO_HEIMER_AXSTATE_LINEAR_PROCESSOR_H

#include <igris/compiler.h>
#include <ralgo/heimer/axstate_signal_processor.h>

namespace heimer
{

    /**
        Преобразователь реализует линейное преобразование путём умножения
       сигнала на матрицу.

        R = M * L
        L = M^-1 * R
    */
    class axstate_linear_processor : public axstate_signal_processor
    {
        //  struct signal_processor proc;
    public:
        double *matrix = nullptr;
        double *invert_matrix = nullptr;

    public:
        axstate_linear_processor() = default;
        axstate_linear_processor(const char *name,
                                 int dim,
                                 axis_state **leftside,
                                 axis_state **rightside,
                                 double *matrix,
                                 double *invert_matrix);
        axstate_linear_processor(const char *name, int dim);

        axstate_linear_processor(const axstate_linear_processor &) = delete;
        axstate_linear_processor &
        operator=(const axstate_linear_processor &) = delete;

        int dim()
        {
            return leftdim();
        }

        int feedback(disctime_t time) override;
        int serve(disctime_t time) override;
        int command(int argc, char **argv, char *output, int outmax) override;
        void deinit() override;

        void on_activate(disctime_t) override;

        void evaluate_invertion();
        void allocate_resources();

        int help(char *output, int outmax) override;
    };
}

#endif
