#ifndef RALGO_HEIMER_POLYGON_CHECKER_H
#define RALGO_HEIMER_POLYGON_CHECKER_H

#include <ralgo/geom/point_relations.h>

namespace heimer
{
    template <class P> class plane_zone_checker : public coordinate_checker<P>
    {
    public:
        linalg::vec<P, 2> *arr;
        int arrcap;
        int arrsize;

        plane_zone_checker(igris::array_view<linalg::vec<P, 2>> arr)
        {
            this->arr = arr.data();
            this->arrcap = arr.size();
            this->arrsize = 0;
        }

        bool
        check_impl(control_node *dev, P *val, int dim, char *msgbuf) override
        {
            assert(dim == 2);

            linalg::vec<P, 2> t(val[0], val[1]);
            bool in = ralgo::point2_in_polygon(t, arr, (size_t)arrsize);

            if (in)
            {
                return 0;
            }

            else
            {
                strcpy(msgbuf, "target is not in permitted polygon");
                return -1;
            }
        }

        int add(linalg::vec<P, 2> pnt)
        {
            if (arrsize == arrcap)
                return -1;

            arr[arrsize++] = pnt;
            return 0;
        }

        int clean()
        {
            arrsize = 0;
            return 0;
        }

        int command(int argc, char **argv) override
        {
            if (argc == 0)
            {
                nos::println("table cappasity:", arrcap);
                nos::println("table size:", arrsize);

                for (int i = 0; i < arrsize; ++i)
                {
                    nos::println(i, ":", arr[i]);
                }

                return 0;
            }

            if (strcmp(argv[0], "clean") == 0)
            {
                return clean();
            }

            if (strcmp(argv[0], "add") == 0)
            {
                if (argc != 3)
                {
                    nos::println("wrong pnt length");
                }

                if (arrsize == arrcap)
                {
                    nos::println("table is full");
                    return -1;
                }

                float a = strtof(argv[1], nullptr);
                float b = strtof(argv[2], nullptr);

                return add({a, b});
            }

            return -1;
        }
    };

    template <class P, class V>
    class polygon_difference_checker : public plane_zone_checker<P>
    {
    private:
        using parent = plane_zone_checker<P>;

        heimer::linintctr_basic<P, V> *positive;
        heimer::linintctr_basic<P, V> *negative;

        float inverse_0, inverse_1;

    public:
        polygon_difference_checker(heimer::linintctr_basic<P, V> *positive,
                                   heimer::linintctr_basic<P, V> *negative,
                                   igris::array_view<linalg::vec<P, 2>> arr,
                                   bool inverse_0 = false,
                                   bool inverse_1 = false)
            : plane_zone_checker<P>(arr), positive(positive),
              negative(negative), inverse_0(inverse_0 ? -1 : 1),
              inverse_1(inverse_1 ? -1 : 1)
        {
            assert(positive != negative);
        }

        bool
        check_impl(control_node *dev, P *val, int dim, char *msgbuf) override
        {
            assert(positive == dev || negative == dev);
            assert(dim == 2);

            linalg::vec<P, 2> t(val[0], val[1]);
            linalg::vec<P, 2> pos_point;
            linalg::vec<P, 2> neg_point;

            positive->current_point((P *)&pos_point);
            negative->current_point((P *)&neg_point);

            auto posgains = positive->gains();
            auto neggains = negative->gains();

            if (positive == dev)
            {
                linalg::vec<P, 2> oposite = {neg_point[0] / neggains[0],
                                             neg_point[1] / neggains[1]};
                t = {t[0] / posgains[0], t[1] / posgains[1]};
                t -= linalg::vec<P, 2>(oposite[0] * inverse_0,
                                       oposite[1] * inverse_1);
            }
            else if (negative == dev)
            {
                linalg::vec<P, 2> oposite = {pos_point[0] / posgains[0],
                                             pos_point[1] / posgains[1]};
                t = {t[0] / neggains[0], t[1] / neggains[1]};
                t -= linalg::vec<P, 2>(oposite[0] * inverse_0,
                                       oposite[1] * inverse_1);
            }

            // auto pnt = pos_point - neg_point;

            bool in_polygon = ralgo::point2_in_polygon(
                t, parent::arr, (size_t)parent::arrsize);

            if (in_polygon)
            {
                return 0;
            }

            else
            {
                strcpy(msgbuf, "target is not in permitted difference polygon");
                return -1;
            }
        }
    };
}

#endif
