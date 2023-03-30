#ifndef GENOS_RESOURCE_RESTBL_H
#define GENOS_RESOURCE_RESTBL_H

#include <genos/resource/openres.h>
#include <vector>

namespace genos
{
    class resource_table
    {
        std::vector<genos::openres> table = {};

    public:
        openres *create_openres()
        {
            table.resize(table.size() + 1);
            return &table[table.size() - 1];
        }

        openres *operator[](size_t i)
        {
            if (i >= table.size())
                return nullptr;

            return &table[i];
        }

        size_t size()
        {
            return table.size();
        }

        void open_new_as(openres &res)
        {
            auto *newres = create_openres();
            newres->open_as(res);
        }
    };
}

#endif