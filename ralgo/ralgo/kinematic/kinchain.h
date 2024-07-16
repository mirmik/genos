#ifndef RALGO_HEIMER_KINEMATIC_CHAIN_H
#define RALGO_HEIMER_KINEMATIC_CHAIN_H

#include <ralgo/space/pose3.h>
#include <ralgo/space/screw.h>

namespace ralgo
{
    /** @brief Расчёт выходных чуствительностей кинематической цепи.
        @detail
            Кинематическая цепь задана выражением
            P_1^n = prod(i=[1,n], C_i W_i(x_i)) C_{n+1}

            P_1^5 = C1 W1(x1) C2 W2(x2) C3 W3(x3) C4 W4(x4) C5
            P_1^5 = P_1^2 W2(x2) P_3^5

            Угловая скорость дифференцируется по формуле Жилина,
            а скорость по формуле Бура.

            d(|R r|)     = |dR/dx dr/dx| = |S*R S*r+d'r/dx| = |S v||R r| / dr/dx
       = S*r+v
             (|0 1|)/dx    |    0     0|   |  0          0|   |0 0||0 1| /

            dd(|R r|)       = |S v| d(|R r|)    = |S v||S v||R r| = |S*S S*v||R
       r|
              (|0 1|)/dxdx    |0 0|  (|0 1|)/dx   |0 0||0 0||0 1|   |  0   0||0
       1|


            dP_1^n/dt = summ P_1^i dW_i(x_i)/dx_i P_{i+1}^n
            dP_1^n/dt = summ P_1^i S_xi W_i P_{i+1}^n
            dP_1^n/dt = summ P_1^i P_n^{i+1}* S_xi

            |A a||S v||B b| = |A a||S*B S*b+v| = |A*S*B A*S*b+A*v| = |?
       A*(cross(s,r2))+A*v| |0 1||0 0||0 1|   |0 1||  0     0|   |    0 0|   |0
       0|

            |A*S*B A*S*b+A*v| = |(As)xAB A*(cross(s,r2))+A*v|
            |    0         0|   |      0                   0|

            |A*S*S*B A*S*S*b+A*S*v| = |(As_2)xAB A*(cross(s,r2))+A*v|
            |      0             0|   |        0                   0|

                ,
            |w|   = |0             |
            |v|     |wx wx b + wx v|
                x

            A Sx At A B

            A S At A S At = A S At A S At A B = (As)x (As)x A B

            A(w)
            A(w x b + v)




        @param constants - вектор матриц C - размер dim + 1.
        @param locsenses - вектор винтов чуствительности W - размер dim.
        @param dim - количество степеней свободы кинематической цепи.
    */
    void kinematic_chain_sensivities(ralgo::pose3<double> *constants,
                                     ralgo::screw3<double> *locsenses,
                                     double *coords,
                                     int dim,
                                     ralgo::screw3<double> *outsenses);
}

#endif
