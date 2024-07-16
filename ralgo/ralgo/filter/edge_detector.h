#ifndef RALGO_EDGE_DETECTOR_H
#define RALGO_EDGE_DETECTOR_H

#include <any>
#include <cmath>

namespace ralgo
{
    enum class EdgeDetectorStatus
    {
        UpdateFallingCandidate,
        UpdateRisingCandidate,
        FallingEvent,
        RisingEvent,
        None
    };

    enum class SignalTone
    {
        Rising,
        Falling,
        NoInited
    };

    class edge_detector
    {
        SignalTone phase = SignalTone::Rising;
        SignalTone last_direction = SignalTone::NoInited;

        double trigger_level = {};

        double start = 0;
        double last = 0;

        // Запретить срабатывание в зоне выше нуля для нисходящих сигналов
        bool prevent_halfspaces_area = false;

        std::any _candidate_privdata = {};

    public:
        edge_detector() = default;
        edge_detector(double trigger_level) : trigger_level(trigger_level) {}

        void init(double trigger_level)
        {
            this->trigger_level = trigger_level;
        }

        void set_prevent_halfspaces_area(bool en)
        {
            prevent_halfspaces_area = en;
        }

        const std::any &candidate_privdata()
        {
            return _candidate_privdata;
        }

        EdgeDetectorStatus serve(double signal)
        {
            EdgeDetectorStatus status = EdgeDetectorStatus::None;
            if (signal == last)
                return EdgeDetectorStatus::None;

            SignalTone direction =
                signal - last > 0 ? SignalTone::Rising : SignalTone::Falling;

            if (last_direction != direction)
            {
                start = signal;

                if (direction == SignalTone::Rising)
                    status = EdgeDetectorStatus::UpdateRisingCandidate;
                else
                    status = EdgeDetectorStatus::UpdateFallingCandidate;
            }

            else
            {
                if (fabs(signal - start) > trigger_level)
                {
                    if (direction == SignalTone::Falling)
                    {
                        if (phase == SignalTone::Rising)
                        {
                            phase = SignalTone::Falling;

                            if (!prevent_halfspaces_area || start > 0)
                                status = EdgeDetectorStatus::FallingEvent;
                        }
                    }

                    if (direction == SignalTone::Rising)
                    {
                        if (phase == SignalTone::Falling)
                        {
                            phase = SignalTone::Rising;

                            if (!prevent_halfspaces_area || start < 0)
                                status = EdgeDetectorStatus::RisingEvent;
                        }
                    }
                }
            }

            last_direction = direction;
            last = signal;

            return status;
        }

        EdgeDetectorStatus serve(double signal,
                                 const std::any &candidate_privdata)
        {
            auto state = serve(signal);
            if (state == EdgeDetectorStatus::UpdateFallingCandidate ||
                state == EdgeDetectorStatus::UpdateRisingCandidate)
            {
                this->_candidate_privdata = candidate_privdata;
            }
            return state;
        }
    };

    using rising_edge_detector = edge_detector;
}

#endif
