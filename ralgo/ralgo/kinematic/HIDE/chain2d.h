#ifndef RALGO_kinematic_CHAIN2D_H
#define RALGO_kinematic_CHAIN2D_H

#include <igris/container/array_view.h>
#include <ralgo/kinematic/link2d.h>

namespace ralgo
{
    class kinematic_chain2d
    {
    public:
        igris::array_view<unit2d *> chain;
        igris::array_view<kinematic_unit2d *> pairs;

    public:
        kinematic_chain2d(){};

        unit2d *out()
        {
            return chain[chain.size() - 1];
        }

        kinematic_chain2d(igris::array_view<unit2d *> chain,
                          igris::array_view<kinematic_unit2d *> pairs)
            : chain(chain), pairs(pairs)
        {
        }

        void setup(igris::array_view<unit2d *> chain,
                   igris::array_view<kinematic_unit2d *> pairs)
        {
            this->chain = chain;
            this->pairs = pairs;
        }

        void collect_chain(unit2d *finallink, unit2d *startlink = nullptr)
        {
            unit2d *link = finallink;
            auto cit = pairs.rbegin();
            auto it = chain.rbegin();

            do
            {
                *it-- = link;
                if (link->iscynem())
                {
                    *cit-- = (kinematic_unit2d *)link;
                }
                link = link->parent;
            } while (link != startlink);
        }

        void update_location()
        {
            for (auto &c : chain)
            {
                c->update_location();
            }
        }

        void update_model_location()
        {
            update_location();
        }

        void sensivity(ralgo::screw2<float> *senses, unit2d *basis)
        {
            auto outtrans = chain[chain.size() - 1]->global_location;

            for (unsigned int i = 0; i < pairs.size(); ++i)
            {
                ralgo::screw2<float> lsens = pairs[i]->sensivity();

                // Для каждого звена расчитываем преобразование
                // от выходному звену.
                auto linktrans = pairs[i]->global_location;
                auto trsf = linktrans.inverse() * outtrans;
                auto itrsf = trsf.inverse();

                // NOTE: радиус берётся именно по прямому преобразованию,
                // а не по обратному, потому что обратное
                // выполняет трансляцию и поворот в другом порядке.
                auto radius = trsf.translation();

                // Приводим вектор чувствительности
                // к выходному звену.
                float wsens = lsens.rotation();
                auto vsens = linalg::cross(wsens, radius) + lsens.translation();

                senses[i] = {wsens, itrsf.rotate(vsens)};
            }

            if (basis != nullptr)
            {
                ralgo::htrans2<float> btrsf = basis->global_location;
                ralgo::htrans2<float> ctrsf = btrsf.inverse() * outtrans;

                for (unsigned int i = 0; i < pairs.size(); ++i)
                {
                    senses[i].lin = ctrsf.rotate(senses[i].lin);
                }
            }
        }
    };
}

#endif
