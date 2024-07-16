#ifndef MUSIC_H
#define MUSIC_H

#include <array>
#include <string>
#include <vector>

struct MusicSign
{
    double freq;
    double duration;
    std::string text;
    int octave_no;
};

std::vector<MusicSign> parse_flipper_music(double bpm,
                                           int default_duration,
                                           int default_octave,
                                           const std::string &music);

#endif