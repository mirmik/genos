#include <rabbit/intersect.h>
#include <rabbit/prim2.h>

using namespace rabbit;

vec2 rabbit::intersect_point_line2_line2(
    const line2eq & line1,
    const line2eq & line2
)
{
	real a = line1.b * line2.c - line2.b * line1.c;
	real b = line2.a * line1.c - line1.a * line2.c;
	real c = line1.a * line2.b - line2.a * line1.b;

	return { a/c, b/c };
}


int rabbit::intersect_point_segm2_segm2(
    const segm2   * segment1,
    const segm2   * segment2,
    vec2 * out_intersection,
    real epsilon
)
{
	vec2 start1 = segment1->apnt;
	//vec2 start2 = segment2->apnt;
	vec2 end1 = segment1->bpnt;
	//vec2 end2 = segment2->bpnt;

	vec2 dir1 = end1 - start1;
	//vec2 dir2 = end2 - start2;

	line2eq eq1 = segment1->line_equation();
	line2eq eq2 = segment2->line_equation();

	// подставляем концы отрезков, для выяснения в каких полуплоскотях они
	// точки первого отрезка подставляются во второе уравнение,
	// а второго в первое.
	real seg1_line2_start = eq2.subs(segment1->apnt);
	real seg1_line2_end = eq2.subs(segment1->bpnt);

	real seg2_line1_start = eq1.subs(segment2->apnt);
	real seg2_line1_end = eq1.subs(segment2->bpnt);

	// если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
	// Проверяем с учетом точности. Если отрезок подходит к другому ближе, чем на эпсилон, защитывается пересечение.
	if (seg1_line2_start * seg1_line2_end >= epsilon || seg2_line1_start * seg2_line1_end >= epsilon)
	{
		return 0;
	}

	// Из пропорции расстояний находим параметр в точке пересечения.
	real u1 = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
	//real u2 = seg2_line1_start / (seg2_line1_start - seg2_line1_end);
	*out_intersection =  start1 + u1 * dir1;

	return 1;
}

int rabbit::__intersect_points_segm2_polysegm2(
    const segm2     * segm,
    const polysegm2 * poly,
    vec2   * points
)
{
	//linalg::vec<real,2> points;//[poly.pnts_count];
	int results_count = 0;

	for (int i = 0; i < poly->pnts_count - 1; ++ i)
	{
		segm2 s(poly->pnts[i], poly->pnts[i + 1]);

		int finded = intersect_point_segm2_segm2(
		                 segm, &s, &points[results_count]);

		results_count += finded;
	}

	return results_count;
}

int rabbit::intersect_points_segm2_polysegm2(
    const segm2     * segm,
    const polysegm2 * poly,
    vec2   * points,
    real epsilon
)
{
	int count = __intersect_points_segm2_polysegm2(
	                segm, poly, points);

	// Фильтруем точки на стыках отрезков.
	int count2 = 1;
	int write_it = 1;

	if (count == 0)
		return 0;

	for (int i = 1; i < count; ++i)
	{
		if (i >= 1 && linalg::length(points[i] - points[i - 1]) >= r_epsilon)
		{
			points[write_it++] = points[i];
			count2++;
		}
	}

	if (count2 != 1 && linalg::length(points[0] - points[count2 - 1]) < r_epsilon)
	{
		count2--;
	}

	return count2;
}
