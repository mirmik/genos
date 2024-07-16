/**
    @file
*/

#ifndef Logging_H
#define Logging_H

#include <mutex>
#include <nos/log.h>

void init_logging_system(std::string_view loglevel, bool force_debug);

extern std::recursive_mutex preset_protector_mutex;

#endif