#ifndef RALGO_INTEGRATE_H
#define RALGO_INTEGRATE_H

namespace ralgo
{
    template <class T, class F>
    T integrate_trapecidal(F f, T begin, T end, int points);

    template <class T, class F>
    T integrate_rectangle(F f, T begin, T end, int points);

    template <class T, class F>
    T integrate_rectangle_left(F f, T begin, T end, int points);

    template <class T, class F>
    T integrate_rectangle_right(F f, T begin, T end, int points);

    template <class T, class F>
    T integrate_parabolic(F f, T begin, T end, int points);
} // namespace ralgo

template <class T, class F>
T ralgo::integrate_trapecidal(F f, T begin, T end, int points)
{
    T cur0 = begin;
    T fcur0 = f(cur0);
    T step = (end - begin) / (points - 1);

    T integ = 0;
    for (int i = 1; i < points; ++i)
    {
        T next = cur0 + step;
        T fnext = f(next);

        integ += (fcur0 + fnext) / 2 * step;

        cur0 = next;
        fcur0 = fnext;
    }

    return integ;
}

template <class T, class F>
T ralgo::integrate_rectangle(F f, T begin, T end, int points)
{
    T step = (end - begin) / (points - 1);
    T cur = begin + step / 2;

    T integ = 0;
    for (int i = 1; i < points; ++i)
    {
        integ += f(cur) * step;
        cur += step;
    }

    return integ;
}

template <class T, class F>
T ralgo::integrate_rectangle_left(F f, T begin, T end, int points)
{
    T step = (end - begin) / (points - 1);
    T cur = begin;

    T integ = 0;
    for (int i = 1; i < points; ++i)
    {
        integ += f(cur) * step;
        cur += step;
    }

    return integ;
}

template <class T, class F>
T ralgo::integrate_rectangle_right(F f, T begin, T end, int points)
{
    T step = (end - begin) / (points - 1);
    T cur = begin + step;

    T integ = 0;
    for (int i = 1; i < points; ++i)
    {
        integ += f(cur) * step;
        cur += step;
    }

    return integ;
}

template <class T, class F>
T ralgo::integrate_parabolic(F f, T begin, T end, int points)
{
    T cur0 = begin;
    T fcur0 = f(cur0);
    T step = (end - begin) / (points - 1);
    T halfstep = step / 2;

    T integ = 0;
    for (int i = 1; i < points; ++i)
    {
        T next = cur0 + step;
        T fnext = f(next);
        T half = cur0 + halfstep;

        integ += (fcur0 + 4 * f(half) + fnext) / 6 * step;

        cur0 = next;
        fcur0 = fnext;
    }

    return integ;
}

/*template <class T>
float rungeKutta(float x0, float y0, float x, float h)
{
    int n = (int)((x - x0) / h);

    float k1, k2, k3, k4, k5;

    float y = y0;
    for (int i = 1; i <= n; i++)
    {
        k1 = h * dydx(x0, y);
        k2 = h * dydx(x0 + 0.5 * h, y + 0.5 * k1);
        k3 = h * dydx(x0 + 0.5 * h, y + 0.5 * k2);
        k4 = h * dydx(x0 + h, y + k3);
        y = y + (1.0 / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);;
        x0 = x0 + h;
    }

    return y;
}*/

#endif
