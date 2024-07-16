#ifndef RALGO_LINE_TRAJECTORY_H
#define RALGO_LINE_TRAJECTORY_H

namespace ralgo
{
    template <class Time, class Position> class LinePositionFunction
    {
        Time start_time = 0;
        Time finish_time = 1;
        Position start_position = 0;
        Position finish_position = 0;

    public:
        LinePositionFunction() = default;

        LinePositionFunction(Time start_time,
                             Time finish_time,
                             Position start_position,
                             Position finish_position)
            : start_time(start_time), finish_time(finish_time),
              start_position(start_position), finish_position(finish_position)
        {
        }

        Position operator()(Time time)
        {
            if (time >= finish_time)
                return finish_position;
            if (time <= start_time)
                return start_position;
            auto k = (double)(time - start_time) /
                     (double)(finish_time - start_time);
            auto result = start_position * (1 - k) + finish_position * (k);
            return result;
        }

        bool in_interval(Time time)
        {
            return time >= start_time && time <= finish_time;
        }
    };
} // namespace ralgo

#endif