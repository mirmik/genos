#ifndef IGRIS_OBJECT_TIME_H
#define IGRIS_OBJECT_TIME_H

namespace igris
{
    class object_time
    {
        long _timeline_zero = 0;
        long _broken_timeline_zero = 0;
        // long _non_finished_offset = 0;
        // long _finished_offset = 0;
        bool _is_reversed = false;

    public:
        void SetReferencePoint(long new_timeline_zero,
                               bool newdirection_is_reversed)
        {
            long dist = new_timeline_zero - _timeline_zero;
            long broken_dist = _is_reversed ? -dist : dist;
            _broken_timeline_zero += broken_dist;
            _timeline_zero = new_timeline_zero;
            _is_reversed = newdirection_is_reversed;
        }

        long TimelineToBroken(long nonbroken_timeline_step)
        {
            long dist = nonbroken_timeline_step - _timeline_zero;
            long broken_dist = _is_reversed ? -dist : dist;
            long broken = broken_dist + _broken_timeline_zero;
            return broken;
        }

        long ToBroken(long s)
        {
            return TimelineToBroken(s);
        }

        long BrokenToTimeline(long broken)
        {
            long broken_dist = broken - _broken_timeline_zero;
            long dist = _is_reversed ? -broken_dist : broken_dist;
            long nonbroken_timeline_step = _timeline_zero + dist;
            return nonbroken_timeline_step;
        }
    };
}

#endif