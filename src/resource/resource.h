#ifndef GENOS_RESOURCE_H
#define GENOS_RESOURCE_H

#include <inttypes.h>
#include <gxx/format.h>

namespace genos {
	class resource {
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

		virtual void resourceOpen() = 0;
		virtual void resourceRelease() = 0;
		virtual void resourceClose() = 0;
	};

	class test_resource : public resource {
		void resourceOpen() override {
			dprf("test_resource_open {} \n", refs);
		}

		void resourceRelease() override {
			dprf("test_resource_release\n");
		}

		void resourceClose() override {
			dprf("test_resource_close\n");
		}
	};
}

#endif