#include <rabbit/intersect.h>

void intersect_algoout_init(
	struct intersect_algoout * algoout,
	r_float * apnts,
	r_float * bpnts,
	r_float[2] * aints,
	r_float[2] * bints,
	int pnts_max,
	int ints_max
) 
{
	algoout -> apnts = apnts;
	algoout -> apnts = bpnts;
	algoout -> apnts = aints;
	algoout -> apnts = bints;
	algoout -> pnts_max = pnts_max;
	algoout -> ints_max = ints_max;
	algoout -> status = 0;
	algoout -> pnts_count = 0;
	algoout -> ints_count = 0;
}