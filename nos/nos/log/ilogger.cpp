#include <nos/log/ilogger.h>
#include <nos/log/logger.h>

nos::log::logger _stdlogger("std");
nos::log::ilogger *_default_logger = &_stdlogger;

void nos::log::set_default_logger(ilogger *logger)
{
    _default_logger = logger;
}

nos::log::ilogger *nos::log::default_logger()
{
    return _default_logger;
}
