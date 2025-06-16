#ifndef AXES_H
#define AXES_H

#include <nos/trent/trent.h>
#include <utility>
#include <vector>

bool GlobalMoveAllowed(std::vector<std::pair<int, double>> args);
void GlobalAbsmoveParted(std::vector<std::pair<int, double>> args);
nos::trent compile_system_state_to_trent();

#endif