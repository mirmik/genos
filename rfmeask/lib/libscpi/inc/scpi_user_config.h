#ifndef SCPI_USER_CONFIG_H
#define SCPI_USER_CONFIG_H

#define LIST_OF_USER_ERRORS                                                    \
    XE(SCPI_ERROR_AXIS_UNREADY, -1000, "Axis is unready")                      \
    XE(SCPI_ERROR_AXIS_OPERATION_TIME, -1001, "Axis: Operation time error")    \
    XE(SCPI_ERROR_DEVICE_UNREADY, -1007, "Device is unready")                  \
    XE(SCPI_ERROR_DEVICE_OPERATION_TIME, -1008,                                \
       "Device: Operation time error")                                         \
    XE(SCPI_ERROR_AXIS_IS_NOT_REGISTRED, -1002, "Axis is not registred")       \
    XE(SCPI_ERROR_DEVICE_IS_NOT_REGISTRED, -1003, "Device is not registred")   \
    XE(SCPI_ERROR_TODO, -1004, "This command is not released yet. TODO.")      \
    XE(SCPI_ERROR_FUNCTION_IS_NOT_SUPPORTED, -1005,                            \
       "Function is not supported")                                            \
    XE(SCPI_ERROR_DEVICE_ALARM_SIGNAL, -1006, "Device in ALARM state")         \
    XE(SCPI_ERROR_AXIS_LIMIT, -1009, "Axis stroke limit")

#endif