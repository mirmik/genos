
#include <array>
#include <cctype>
#include <igris/util/string.h>
#include <music.h>
#include <string_view>

std::array<std::array<double, 7>, 9> octave{
    {32.703, 36.708, 41.203, 43.654, 48.999, 55.000, 61.735}, // 1
    {65.406, 73.416, 82.407, 87.307, 97.999, 110.00, 123.47}, // 2
    {130.81, 146.83, 164.81, 174.61, 196.00, 220.00, 246.94}, // 3
    {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88}, // 4
    {523.25, 587.33, 659.25, 698.46, 783.99, 880.00, 987.77}, // 5
    {1046.50, 1174.66, 1318.51, 1396.91, 1567.98, 1760.00, 1975.53},
    {2093.00, 2349.32, 2637.02, 2793.83, 3135.96, 3520.00, 3951.07},
    {4186.01, 4698.63, 5274.04, 5587.65, 6271.93, 7040.00, 7902.13},
    {8372.02, 9397.27, 10548.08, 11175.30, 12543.85, 14080.00, 15804.27}};

// девятая нота лишняя
std::array<std::array<double, 7>, 9> octave_sharp{
    {34.648, 38.891, 43.654, 46.249, 51.913, 58.270, 65.406},
    {69.296, 77.782, 87.307, 92.499, 103.83, 116.54, 130.81},
    {138.59, 155.56, 174.61, 185.00, 207.65, 233.08, 261.63},
    {277.18, 311.13, 349.23, 369.99, 415.30, 466.16, 523.25},
    {554.37, 622.25, 698.46, 739.99, 830.61, 932.33, 1046.50},
    {1108.73, 1244.51, 1396.91, 1479.98, 1661.22, 1864.66, 2093.00},
    {2217.46, 2489.02, 2793.83, 2959.96, 3322.44, 3729.31, 4186.01},
    {4434.92, 4978.03, 5587.65, 5919.91, 6644.88, 7458.62, 8372.02}};

int note2number(char note)
{
    switch (note)
    {
    case 'C':
        return 0;
    case 'D':
        return 1;
    case 'E':
        return 2;
    case 'F':
        return 3;
    case 'G':
        return 4;
    case 'A':
        return 5;
    case 'B':
        return 6;
    default:
        return -1;
    }
}

MusicSign parse_sign(const std::string_view &sign,
                     double bpm,
                     int default_duration,
                     int default_octave)
{
    double note_duration_ms = 60000.0 / bpm * 4;

    default_octave = default_octave - 1;

    MusicSign music_sign;
    music_sign.text = std::string(sign.data(), sign.size());
    const char *it = sign.begin();

    if (isdigit(*it))
    {
        char buf[10];
        memset(buf, 0, 10);
        char *ptr = buf;

        while (isdigit(*it))
        {
            *ptr++ = *it++;
        }
        int n = igris_atoi32(buf, 10, nullptr);
        auto duration_div = n;
        music_sign.duration = note_duration_ms / duration_div;
    }
    else
    {
        music_sign.duration = note_duration_ms / default_duration;
    }

    char note = *it++;
    int note_number = note2number(note);

    if (note_number == -1)
    {
        music_sign.freq = 0;
        return music_sign;
    }

    bool sharp = *it == '#';
    if (sharp)
        ++it;

    int octave_no;
    if (it == sign.end() || !isdigit(*it))
    {
        octave_no = default_octave;
    }
    else
    {
        octave_no = *it - '0' - 1;
        it++;
    }
    music_sign.octave_no = octave_no + 1;

    if (sharp)
    {
        music_sign.freq = octave_sharp[octave_no][note_number];
    }
    else
    {
        music_sign.freq = octave[octave_no][note_number];
    }

    while (it != sign.end())
    {
        if (*it == '.')
        {
            music_sign.duration *= 1.5;
        }
        ++it;
    }

    return music_sign;
};

std::vector<MusicSign> parse_flipper_music(double bpm,
                                           int default_duration,
                                           int default_octave,
                                           const std::string &music)
{
    std::vector<MusicSign> music_signs;
    auto splited = igris::split(music, ',');
    for (auto &sign : splited)
    {
        auto trimmed = igris::trim(sign);
        music_signs.push_back(
            parse_sign(trimmed, bpm, default_duration, default_octave));
    }
    return music_signs;
}