#ifndef RABBIT_PHYSICS_SPEED_H
#define RABBIT_PHYSICS_SPEED_H

namespace rabbit 
{
	struct hspeed3 : public screw3
	{
		hspeed3(const screw3& screw) screw3(screw) {}		
	}
}

#endif