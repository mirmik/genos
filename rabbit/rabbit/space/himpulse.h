#ifndef RALGO_SPACE_HIMPULSE_H
#define RALGO_SPACE_HIMPULSE_H

namespace ralgo 
{
	struct himpulse3 : public screw3 
	{
		// Интегрирование для инерциальной системы отсчета.
		himpulse3 integrate(const haccel3& acc, T delta) 
		{
			return himpulse3(*this + acc * delta);
		}
	}
}

#endif