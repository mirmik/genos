#ifndef GENOS_RESOURCE_RESTBL_H
#define GENOS_RESOURCE_RESTBL_H

#include <genos/resource/openres.h>
#include <igris/container/static_vector.h>
#include <vector>

namespace genos
{
    class resource_table
    {
        igris::static_vector<genos::openres, 5> table = {};

    public:
        openres &create_openres()
        {
            table.emplace_back();
            return table.back();
        }

        int create_openres_fd()
        {
            table.emplace_back();
            return table.size() - 1;
        }

        openres &operator[](size_t i)
        {
            assert(i < table.size());
            return table[i];
        }

        size_t size()
        {
            return table.size();
        }

        void open_new_as(openres &res)
        {
            auto &newres = create_openres();
            newres.open_as(res);
        }
    };
}

#endif