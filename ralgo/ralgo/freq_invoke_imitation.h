#ifndef RALGO_FREQ_INVOKE_IMITATION_H
#define RALGO_FREQ_INVOKE_IMITATION_H

#include <chrono>
#include <functional>
#include <initializer_list>
#include <ranges>
#include <vector>

namespace ralgo
{
    class freq_invoke_imitation
    {
        std::vector<std::pair<std::chrono::nanoseconds, std::function<void()>>>
            invokes = {};
        std::vector<std::chrono::nanoseconds> awake = {};

    public:
        freq_invoke_imitation(
            const std::initializer_list<std::pair<std::chrono::nanoseconds,
                                                  std::function<void()>>> &list)
            : invokes(list)
        {
            std::transform(invokes.begin(),
                           invokes.end(),
                           std::back_inserter(awake),
                           [](const auto &a) { return a.first; });
        }

        std::chrono::nanoseconds minimal_awake()
        {
            std::chrono::nanoseconds minawake =
                std::chrono::nanoseconds(std::numeric_limits<int64_t>::max());

            for (auto a : awake)
                if (a < minawake)
                    minawake = a;

            return minawake;
        }

        void timeskip(std::chrono::nanoseconds ns)
        {
            while (ns.count() != 0)
            {
                auto skip = std::min(minimal_awake(), ns);
                ns -= skip;
                for (size_t i = 0; i < awake.size(); ++i)
                {
                    awake[i] -= skip;
                    if (awake[i].count() <= 0)
                    {
                        awake[i] += invokes[i].first;
                        invokes[i].second();
                    }
                }
            }
        }
    };
}

#endif