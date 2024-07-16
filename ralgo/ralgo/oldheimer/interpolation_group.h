#ifndef RALGO_OLDHEIMER_INTERPOLATION_GROUP_H
#define RALGO_OLDHEIMER_INTERPOLATION_GROUP_H

#include <igris/binreader.h>

#include <ralgo/linalg/linalg.h>
#include <ralgo/oldheimer/control.h>
#include <ralgo/oldheimer/coordinate_checker.h>

namespace heimer
{
    template <class Position, class Speed>
    class linintctr_basic : public control_node
    {
    public:
        bool enable_full_spattern = false;
        heimer::coordinate_checker<Position> *coord_checker = nullptr;
        igris::delegate<void, linintctr_basic *> operation_finish_signal = {};
        igris::delegate<void, linintctr_basic *> operation_start_signal = {};

    public:
        linintctr_basic(const char *name) : control_node(name) {}
        linintctr_basic(const linintctr_basic &) = delete;
        linintctr_basic &operator=(const linintctr_basic &) = delete;

        virtual int incmove(Position *mov) = 0;
        virtual int absmove(Position *pos) = 0;
        virtual int parted_absmove(int *axnums, Position *pos, int len) = 0;
        // virtual int stop() = 0;

        virtual int set_speed(Speed spd) = 0;
        virtual int set_accdcc(Speed acc, Speed dcc) = 0;
        virtual int dim() = 0;

        virtual void current_point(Position *) = 0;

        int set_accdcc(Speed acc)
        {
            return set_accdcc(acc, acc);
        }

        virtual Speed speed() = 0;
        virtual Speed acceleration() = 0;
        virtual Speed deceleration() = 0;

        virtual void set_gains(igris::array_view<float> arr) = 0;
        virtual void set_reverses(igris::array_view<int> arr) = 0;
        virtual float *gains() = 0;

        int command(int argc, char **argv)
        {
            int sts;
            TEMPORARY_STORAGE(float, dim(), fltargs);

            // parted absolute move
            if (strcmp(argv[0], "pmov") == 0)
            {
                TEMPORARY_STORAGE(int, dim(), axnums);

                if (argc > dim() + 1)
                {
                    nos::println("wrong args count");
                    return -1;
                }

                for (int i = 0; i < argc - 1; ++i)
                {
                    igris::binreader reader(argv[1 + i]);

                    sts = reader.read_ascii_decimal_integer(&axnums[i]);
                    if (sts)
                        return -1;

                    reader.skip(1);

                    sts = reader.read_ascii_decimal_float(&fltargs[i]);
                    if (sts)
                        return -1;
                }

                return parted_absmove(
                    std::data(axnums), std::data(fltargs), argc - 1);
            }

            // absolute move
            if (strcmp(argv[0], "mov") == 0)
            {
                if (argc != dim() + 1)
                {
                    nos::println("wrong args count");
                    return -1;
                }

                for (int i = 0; i < dim(); ++i)
                {
                    fltargs[i] = strtof(argv[1 + i], nullptr);
                }

                return absmove(std::data(fltargs));
            }

            if (strcmp(argv[0], "incmov") == 0)
            {
                if (argc != dim() + 1)
                {
                    nos::println("wrong args count");
                    return -1;
                }

                for (int i = 0; i < dim(); ++i)
                {
                    fltargs[i] = strtof(argv[1 + i], nullptr);
                }

                return incmove(std::data(fltargs));
            }

            else if (strcmp(argv[0], "setgain") == 0)
            {
                if (argc != dim() + 1)
                {
                    nos::println("wrong args count");
                    return -1;
                }

                for (int i = 0; i < dim(); ++i)
                {
                    fltargs[i] = strtof(argv[1 + i], nullptr);
                }

                set_gains({std::data(fltargs), (size_t)dim()});
                return 0;
            }

            else if (strcmp(argv[0], "setspd") == 0)
            {
                auto fltarg = strtof(argv[1], nullptr);
                set_speed(fltarg);
                return 0;
            }

            else if (strcmp(argv[0], "setacc") == 0)
            {
                auto fltarg = strtof(argv[1], nullptr);
                set_accdcc(fltarg, fltarg);
                return 0;
            }

            else if (strcmp(argv[0], "feed") == 0)
            {
                print_info();
            }

            else if (strcmp(argv[0], "stop") == 0)
            {
                stop();
            }

            else if (strcmp(argv[0], "mprotect") == 0)
            {
                if (coord_checker != nullptr)
                {
                    return coord_checker->command(argc - 1, argv + 1);
                }

                else
                {
                    nos::println("mprotection is not binded\r\n");
                    return 0;
                }
            }

            else
            {
                nos::println("warn: unresolved command");
            }

            return 0;
        }

        virtual int hardstop() = 0;
        virtual int stop() = 0;

        // virtual bool can_operate() = 0;
        virtual void print_info() = 0;
    };
}

#endif
