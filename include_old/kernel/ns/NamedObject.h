#ifndef GENOS_NAMED_OBJECT_H
#define GENOS_NAMED_OBJECT_H

#include <utilxx/setget.h>

namespace Genos {
	class NamedObject {
		const char* m_name;

	public:
		ACCESSOR(name, m_name);
	};
}

#endif