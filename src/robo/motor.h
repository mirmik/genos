#ifndef GENOS_ROBO_MOTOR_H
#define GENOS_ROBO_MOTOR_H

namespace genos {
	namespace robo {
		struct motor {
			virtual void power(float pwr) = 0;
			virtual void stop() { power(0); }
		};
	}
}

#endif