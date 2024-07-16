#ifndef RABBIT_SYMMETRIC_MATRIX_H
#define RABBIT_SYMMETRIC_MATRIX_H

#include <nos/print.h>
#include <nos/fprint.h>

namespace rabbit 
{
	template <class T, int N>
	class symmetric_matrix;

	template <class T>
	class symmetric_matrix<T,3> 
	{
		T xx, yy, zz, xy, yz, zx;

	public:
		/*symmetric_matrix<T,3> operator * (const symmetric_matrix<T,3>& oth) 
		{
			return 
			{
				xx * oth.xx + xy * oth.xy + zx * oth.zx,
				yy * oth.yy + xy * oth.xy + yz * oth.yz,
				zz * oth.zz + yz * oth.yz + zx * oth.zx,

				xx * oth.xy + xy * oth.yy + zx * oth.yz,
				xy * oth.xz + yy * oth.yz + yz * oth.zz,
				xx * oth.zx + xy * oth.yz + zx * oth.zz
			};

			[xx_a*xx_b + xy_a*xy_b + xz_a*xz_b, 
				xx_a*xy_b + xy_a*yy_b + xz_a*yz_b, 
				xx_a*xz_b + xy_a*yz_b + xz_a*zz_b],
			[xx_b*xy_a + xy_b*yy_a + xz_b*yz_a, 
				xy_a*xy_b + yy_a*yy_b + yz_a*yz_b, 
				xy_a*xz_b + yy_a*yz_b + yz_a*zz_b],
			[xx_b*xz_a + xy_b*yz_a + xz_b*zz_a, 
				xy_b*xz_a + yy_b*yz_a + yz_b*zz_a, 
				xz_a*xz_b + yz_a*yz_b + zz_a*zz_b]])

		}*/

		ssize_t print_to(nos::ostream& o) const 
		{
			return nos::fprint_to(o, 
				"({} {} {} {} {} {})", xx, yy, zz, xy, yz, zx);
		} 	
	};
}

#endif