#ifndef RALGO_LINALG_GPSSOLVER_H
#define RALGO_LINALG_GPSSOLVER_H

#include <ralgo/linalg/linalg.h>
#include <ralgo/linalg/vector.h>
#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/svd.h>

namespace ralgo 
{
    static inline linalg::vec<double, 4> 
    gps_solver(std::vector<linalg::vec<double, 4>> & points, 
    const double c = 2.99792458e8) 
    {
        ralgo::matrix<double> A(points.size(), 4);
        ralgo::vector<double> b(points.size());
        linalg::vec<double,4> r(points.size());

        for (size_t i = 0; i < points.size() - 1; ++i) 
        {
            auto j = (i + 1) % points.size();
            auto x = points[i][0] - points[j][0];
            auto y = points[i][1] - points[j][1];
            auto z = points[i][2] - points[j][2];
            auto t = points[i][3] - points[j][3];
            auto x2 = points[i][0]*points[i][0] - points[j][0]*points[j][0];
            auto y2 = points[i][1]*points[i][1] - points[j][1]*points[j][1];
            auto z2 = points[i][2]*points[i][2] - points[j][2]*points[j][2];
            auto t2 = points[i][3]*points[i][3] - points[j][3]*points[j][3];
            
            b[i] = -x2 - y2 - z2 + t2*c*c;
            A(i, 0) = -2*x;
            A(i, 1) = -2*y;
            A(i, 2) = -2*z;
            A(i, 3) = +2*t*c*c; 
        }

        auto svd_decomposition = ralgo::svd(A);
        svd_decomposition.solve(b, r);
        auto x = r[0];
        auto y = r[1];
        auto z = r[2];
        auto x1 = points[0][0];
        auto y1 = points[0][1];
        auto z1 = points[0][2]; 
        auto t1 = points[0][3];

        // уточняем время, потому что при равенстве временных меток расчёт 
        // последней переменной может быть неверным
        r[3] = sqrt((x - x1)*(x - x1) + (y - y1)*(y - y1) + (z - z1)*(z - z1)) / c + t1;
        
        return r;
    }
}

#endif