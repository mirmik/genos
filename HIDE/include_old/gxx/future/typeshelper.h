#ifndef GXX_TYPES_HELPER_H
#define GXX_TYPES_HELPER_H

#include <inttypes.h>

struct u8u16_t {
	uint8_t a;
	uint16_t b;

	template <typename Archive>
    void load(Archive& ar) {
        a << ar;
        b << ar;
    };

    template <typename Archive>
    void save(Archive& ar) {
        a >> ar;
        b >> ar;
    };
};

#endif