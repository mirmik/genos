#ifndef GENOS_RESOURCE_H
#define GENOS_RESOURCE_H

#include <inttypes.h>
#include <gxx/format.h>

namespace genos {
	class test_resource {
	public:
		uint8_t refs;

		void getref() {
			refs++;
		}

		void putref() {
			refs--; 
			if (refs == 0) resourceClose();
		}

		void open() {
			getref();
			resourceOpen();
		};

		void release() {
			resourceRelease();
			putref();
		};

		void resourceOpen() {
			dprf("test_resource_open {} \n", refs);
		}

		void resourceRelease(){
			dprf("test_resource_release\n");
		}

		void resourceClose(){
			dprf("test_resource_close\n");
		}
	};
}

#endif