#ifndef RABBIT_INTERSECT_H
#define RABBIT_INTERSECT_H

#include <rabbit/third/linalg.h>
#include <rabbit/types.h>
#include <rabbit/prim2.h>

namespace rabbit
{
/*	class intersect_node
	{
		bool is_interval;
		real aparam0;
		real aparam1;
		real bparam0;
		real bparam1;
	};
*/
	int intersect_point_segm2_segm2(
	    const segm2   * segment1,
	    const segm2   * segment2,
	    vec2 * out_intersection,
	    real epsilon = rabbit::epsilon
	);

	int __intersect_points_segm2_polysegm2(
	    const segm2     * segm,
	    const polysegm2 * poly,
	    vec2   * points
	);

	int intersect_points_segm2_polysegm2(
	    const segm2     * segm,
	    const polysegm2 * poly,
	    vec2   * points,
	    real epsilon = rabbit::epsilon
	);


	vec2 intersect_point_line2_line2(
	    const line2eq & line1,
	    const line2eq & line2
	);
}

#endif