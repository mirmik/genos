#ifndef GENOS_STREAM_SERVICE_H
#define GENOS_STREAM_SERVICE_H

#include <kernel/service/service.h>
#include <kernel/service/srvtask.h>
//#include <proto/stream.h>

namespace genos {

	template<typename ProtoStream> 
	class stream_service : public service, public ProtoStream {
	public:
		class input_tasklet : public service_tasklet<stream_service> {
			input_tasklet(stream_service* parent, uint8_t prio) :
				genos::service_tasklet<stream_service>(parent, prio) {}; 
		};
	
		class output_tasklet : public service_tasklet<stream_service> {
			output_tasklet(stream_service* parent, uint8_t prio) :
				genos::service_tasklet<stream_service>(parent, prio) {}; 
		};

	public:
		//Stream* strm;

		input_tasklet 	itask;
		output_tasklet 	otask;

		stream_service() {

		}		
	};

}

#endif