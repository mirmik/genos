#ifndef RABBIT_EXTREMA_H
#define RABBIT_EXTREMA_H

namespace rabbit 
{
	class extrema_parametric_curve2_uniform_gradient_search 
	{
	private:
		parametric_curve2 * acurve;
		parametric_curve2 * bcurve;

		double astep;
		double bstep;

	public:
		real metric(real a, real b) 
		{
			apnt = acurve->d0(a);
			bpnt = bcurve->d0(b);
			return rabbit::distance(apnt, bpnt);
		}

		void init() {}
		
	public:
		extrema_parametric_curve2_uniform_gradient_search(
			parametric_curve2 * acurve,
			parametric_curve2 * bcurve
		) :
			acurve(acurve),
			bcurve(bcurve)
		{}

		void 	
	}

	class extrema_parametric_curve2 
	{
	private:
		parametric_curve2 * acurve;
		parametric_curve2 * bcurve;
		
	public:
		extrema(
			parametric_curve2 * acurve,
			parametric_curve2 * bcurve
		) :
			acurve(acurve),
			bcurve(bcurve)
		{}
		

	}
}

#endif