#ifndef RFMEAS_CONFIG_H
#define RFMEAS_CONFIG_H

/**
    @file
*/

#include <nos/trent/json_settings.h>

extern nos::json_settings config_settings;
extern nos::json_settings objects_runtime;
extern nos::json_syncer runtime_syncer;
void config_load(std::string str, std::string runtime);

#endif
