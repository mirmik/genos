#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    /*	template <class V>
        linalg::mat<V, 2, 2> exponent(const linalg::mat<V, 2, 2> A)
        {
            V a = A[0][0];
            V b = A[0][1];
            V c = A[1][0];
            V d = A[1][1];

            V o = b*exp(a/2 + c/2 - sqrt(a**2 - 2*a*c + 4*b**2 +
       c**2)/2)/((b/(a/2 - c/2 + sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2) - b/(a/2
       - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2))*(a/2 - c/2 + sqrt(a**2 -
       2*a*c + 4*b**2 + c**2)/2)) - b*exp(a/2 + c/2 + sqrt(a**2 - 2*a*c + 4*b**2
       + c**2)/2)/((b/(a/2 - c/2 + sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2) -
       b/(a/2 - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2))*(a/2 - c/2 -
       sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2)); V p = b**2*exp(a/2 + c/2 -
       sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2)/((b/(a/2 - c/2 + sqrt(a**2 - 2*a*c
       + 4*b**2 + c**2)/2) - b/(a/2 - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 +
       c**2)/2))*(a/2 - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2)*(a/2 - c/2 +
       sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2)) - b**2*exp(a/2 + c/2 + sqrt(a**2 -
       2*a*c + 4*b**2 + c**2)/2)/((b/(a/2 - c/2 + sqrt(a**2 - 2*a*c + 4*b**2 +
       c**2)/2) - b/(a/2 - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2))*(a/2 -
       c/2 - sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2)*(a/2 - c/2 + sqrt(a**2 -
       2*a*c + 4*b**2 + c**2)/2)); V r = -exp(a/2 + c/2 - sqrt(a**2 - 2*a*c +
       4*b**2 + c**2)/2)/(b/(a/2 - c/2 + sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2) -
       b/(a/2 - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2)) + exp(a/2 + c/2 +
       sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2)/(b/(a/2 - c/2 + sqrt(a**2 - 2*a*c +
       4*b**2 + c**2)/2) - b/(a/2 - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 +
       c**2)/2)); V s = b*exp(a/2 + c/2 + sqrt(a**2 - 2*a*c + 4*b**2 +
       c**2)/2)/((b/(a/2 - c/2 + sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2) - b/(a/2
       - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2))*(a/2 - c/2 + sqrt(a**2 -
       2*a*c + 4*b**2 + c**2)/2)) - b*exp(a/2 + c/2 - sqrt(a**2 - 2*a*c + 4*b**2
       + c**2)/2)/((b/(a/2 - c/2 + sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2) -
       b/(a/2 - c/2 - sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2))*(a/2 - c/2 -
       sqrt(a**2 - 2*a*c + 4*b**2 + c**2)/2));

            return {{o,r}, {p,s}};
        }*/
}
