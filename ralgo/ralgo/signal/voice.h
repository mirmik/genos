#ifndef RALGO_SIGNAL_VOICE_H
#define RALGO_SIGNAL_VOICE_H

#include <ralgo/vecops.h>

namespace ralgo
{
    static inline constexpr double hz2mel(double hz)
    {
        return 1127 * log(1 + (hz / 700));
    }
    static inline constexpr double mel2hz(double mel)
    {
        return 700 * (exp(mel / 1127) - 1);
    }

    template <typename V> V hz2mel_v(const V &vec)
    {
        return elementwise(vec, hz2mel);
    }
    template <typename V> V mel2hz_v(const V &vec)
    {
        return elementwise(vec, mel2hz);
    }

    template <typename V> V &hz2mel_vi(V &vec)
    {
        return inplace::elementwise(vec, hz2mel);
    }
    template <typename V> V &mel2hz_vi(V &vec)
    {
        return inplace::elementwise(vec, mel2hz);
    }
}

#endif
