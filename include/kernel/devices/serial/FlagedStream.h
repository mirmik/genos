#ifndef GENOS_FLAGED_STREAM_H
#define GENOS_FLAGED_STREAM_H

#include <kernel/event/StateFlag.h>
#include <kernel/devices/serial/Stream.h>

namespace Genos {
	class FlagedStream : public Stream {
	public:
		StateFlag haveDataFlag;
	};
}

#endif