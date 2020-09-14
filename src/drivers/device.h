#ifndef GENOS_DEVICE_H
#define GENOS_DEVICE_H

// Базовый класс переферийного устройства.

namespace genos
{
	class device 
	{
		virtual device * parent_device() = 0;
	};


}

#endif