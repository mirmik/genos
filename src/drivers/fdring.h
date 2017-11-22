#ifndef GENOS_FDRING_STORAGE_H
#define GENOS_FDRING_STORAGE_H

#include <gxx/io/iostorage.h>
#include <gxx/bytering.h>

namespace gxx {
	namespace io {
		class fdring : public gxx::io::ostream, public gxx::io::istorage {
		public:

			gxx::bytering rxring;
		
		};
	}
}

#endif