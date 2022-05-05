#ifndef GENOS_RESOURCE_RESTBL_H
#define GENOS_RESOURCE_RESTBL_H

#include <genos/resource/openres.h>
#include <vector>

namespace genos 
{
	class resource_table 
	{
		std::vector<genos::openres> table;
		
	public:
		openres* get_resource() 
		{
			table.resize(table.size() + 1);
			return &table.back();
		}
	};
}

/*struct restbl_head 
{
	struct openres * table;
	int              total;
};

__BEGIN_DECLS
__END_DECLS

#define RESOURCE_TABLE(name, total) \
struct openres name##_table_array[total];  \
struct restbl_head name = { name##_table_array, total };*/

#endif