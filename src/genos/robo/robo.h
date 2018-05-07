#ifndef GENOS_ROBO_H
#define GENOS_ROBO_H

#include <gxx/event/delegate.h>

namespace genos {
	namespace robo {
		struct power_driver {
			virtual double power();
			virtual void power(double procent);
		};

		struct switch_button {
			bool reversed;
			bool last_status;
			
			uint8_t trigger_value;
			uint8_t trigger_treshold;
			uint8_t trigger_high;
			
			gxx::delegate<void, bool> status_changed;
			virtual void update_status();
			void bool sync_status();
		};

		template <typename PosType>
		struct position_sensor {
			virtual void update_position();
			PosType sync_position();
			gxx::delegate<void, PosType> position_updated;
		};
	}
}

#endif