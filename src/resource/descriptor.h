#ifndef GENOS_DESCRIPTOR_H
#define GENOS_DESCRIPTOR_H

#include <inttypes.h>
#include <gxx/debug/dprint.h>
#include <resource/resource.h>

namespace genos {
	class descriptor {
	public:
		enum class DescType : uint8_t {
			Nil = 0,
			File = 1,
			Messenger = 2,
			TestResource = 3,
			UserResource = 4,
		};

		DescType type;
		uint16_t flags;
		void* body;

		union {
			struct {
				size_t fpos;
			};
		};

		//descriptor() : type(DescType::Nil), flags(0), body(nullptr) {};

		descriptor(DescType type, uint16_t flags, void* body) : 
			type(type), flags(flags), body(body) {};
	};

	//void close(int descno);
	descriptor* get_descriptor(int no);
	void close_descriptor(descriptor& desc);
	int open_test_resource(test_resource& res);
}

#endif