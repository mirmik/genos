#ifndef RALGO_FILTER_PID_H
#define RALGO_FILTER_PID_H

namespace ralgo
{
    class pi_filter
    {
        double _kp, _ki;
        double _integral = 0;

        bool _limited = false;
        double _integral_lo = 0;
        double _integral_hi = 0;

    public:
        pi_filter() : _kp(0), _ki(0) {}

        pi_filter(double kp, double ki) : _kp(kp), _ki(ki) {}

        void init(double kp, double ki)
        {
            _kp = kp;
            _ki = ki;
        }

        double integral()
        {
            return _integral;
        }

        void reset_integral(double val = 0)
        {
            _integral = val;
        }

        void set_integral_limits(double lo, double hi)
        {
            _integral_lo = lo;
            _integral_hi = hi;
            _limited = true;
        }

        double serve(double val, double delta)
        {
            _integral += val * delta;
            return _kp * val + _ki * _integral;
        }
    };
}

#endif
