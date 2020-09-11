#ifndef GENOS_DEVICE_H
#define GENOS_DEVICE_H

// Базовый класс переферийного устройства.

namespace genos
{
	class device 
	{
	public:
		device * parent;
	
		device() : parent(nullptr) {}
		device(device * par) : parent(par) {}
	};
}

#endif