#ifndef GENOS_DRIVERS_PWM_SERVO_H
#define GENOS_DRIVERS_PWM_SERVO_H

#include <assert.h>

template <typename T>
class pwmservo 
{
	volatile T* reg;

	float min;
	float max;
	float minmax;

	T regmin;
	T regmax;

public:
	pwmservo(volatile T* reg, float min, float max, T regmax, T regmin) : 
		reg(reg), min(min), max(max), minmax(max - min), regmin(regmin), regmax(regmax) {}

	void set_value(float f) 
	{
		assert(f >= min && f <= max);
		float p = (f - min) / minmax;
		set(p);
	}

	void set(float p) 
	{
		assert(p >= 0 && p <= 1);
		*reg = (float)regmin * (1-p) + (float)regmax * p;		
	}
};

#endif