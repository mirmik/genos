#pragma once

#include "igris_std_config.hpp"

#ifdef IGRIS_STD_AS_STD
// Позволяем старому коду с именем igris_std продолжить работать,
// даже если реализация подключена напрямую в std.
namespace igris_std = std;
#else
namespace igris_std {}
#endif
