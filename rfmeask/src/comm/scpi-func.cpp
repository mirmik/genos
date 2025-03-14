/**
    @file
*/

#include <AbstractAxis.h>
#include <AxisSynchro.h>
#include <CommonAxis.h>
#include <SyncScanApiClient.h>
#include <System.h>
#include <axes_api.h>
#include <comm/scpi-func.h>
#include <devices/DeviceChecker.h>
#include <groups/InterpolationGroup.h>
#include <igris/util/numconvert.h>
#include <igris/util/string.h>
#include <moveapi/CNCMovingClient.h>
#include <ralgo/linalg/vecops.h>
#include <scpi/scpi.h>
#include <scriptexec.h>
#include <servos/AbstractServo.h>
#include <tables.h>

#define TRY_BEGIN                                                              \
    try                                                                        \
    {
#define TRY_END                                                                \
    }                                                                          \
    catch (const AxisLimitException &excpt)                                    \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_AXIS_LIMIT);                        \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const AxisException &excpt)                                         \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_AXIS_UNREADY);                      \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const DeviceException &excpt)                                       \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_UNREADY);                    \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const FunctionIsNotSupported &excpt)                                \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_FUNCTION_IS_NOT_SUPPORTED);         \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const FunctionIsNotReleased &excpt)                                 \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_TODO);                              \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const WrongValueException &excpt)                                   \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);           \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const std::length_error &excpt)                                     \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_PARAMETER_ERROR);                   \
        return SCPI_RES_ERR;                                                   \
    }

#define TRY_END_Q                                                              \
    }                                                                          \
    catch (const AxisLimitException &excpt)                                    \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_AXIS_LIMIT);                        \
        scpi_args_funcs<Ret>::send(context, 0);                                \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const AxisException &excpt)                                         \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_AXIS_UNREADY);                      \
        scpi_args_funcs<Ret>::send(context, 0);                                \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const DeviceException &excpt)                                       \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_UNREADY);                    \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const FunctionIsNotSupported &excpt)                                \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_FUNCTION_IS_NOT_SUPPORTED);         \
        scpi_args_funcs<Ret>::send(context, 0);                                \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const FunctionIsNotReleased &excpt)                                 \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_TODO);                              \
        scpi_args_funcs<Ret>::send(context, 0);                                \
        return SCPI_RES_ERR;                                                   \
    }                                                                          \
    catch (const WrongValueException &excpt)                                   \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);           \
        scpi_args_funcs<Ret>::send(context, 0);                                \
        return SCPI_RES_ERR;                                                   \
    }

#define ILLEGAL_PARAMETR_VALUE_RETURN                                          \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);           \
        return SCPI_RES_ERR;                                                   \
    }

#define PROTECT_AXIS_NUMBER(number)                                            \
    if ((unsigned int)number >= axes_total())                                  \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_AXIS_IS_NOT_REGISTRED);             \
        return SCPI_RES_ERR;                                                   \
    }

#define PROTECT_DEVICE_NUMBER(number)                                          \
    if ((unsigned int)number >= devices_list.size())                           \
    {                                                                          \
        SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_IS_NOT_REGISTRED);           \
        return SCPI_RES_ERR;                                                   \
    }

scpi_result_t SCPIFUNC_totalAxesQ(scpi_t *context)
{
    SCPI_ResultInt32(context, axes_total());
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_totalIntGroupsQ(scpi_t *context)
{
    SCPI_ResultInt32(context, groups_total());
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_totalDevicesQ(scpi_t *context)
{
    SCPI_ResultInt32(context, devices_list.size());
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_SystemPreset(scpi_t *context)
{
    TRY_BEGIN
    for (auto a : get_axis_list())
    {
        a->preset();
    }
    TRY_END
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_SystemStop(scpi_t *context)
{
    TRY_BEGIN
    script_terminate_all();

    for (auto a : get_axis_list())
    {
        a->stop();
    }
    TRY_END
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_SystemPowerOff(scpi_t *context)
{
    TRY_BEGIN
    for (auto a : get_axis_list())
    {
        a->servo_off();
    }
    TRY_END
    return SCPI_RES_OK;
}

scpi_bool_t SCPI_ParamInt32_Bool(scpi_t *a, uint32_t *b, scpi_bool_t c)
{
    return SCPI_ParamInt32(a, (int32_t *)b, c);
}

size_t SCPI_ResultInt32_Bool(scpi_t *a, uint32_t b)
{
    return SCPI_ResultInt32(a, (int32_t)b);
}

template <typename Arg> struct scpi_args_funcs
{
};
template <> struct scpi_args_funcs<int32_t>
{
    constexpr static scpi_bool_t (*recv)(scpi_t *,
                                         int32_t *,
                                         scpi_bool_t) = SCPI_ParamInt32;
    constexpr static size_t (*send)(scpi_t *, int32_t val) = SCPI_ResultInt32;
};
template <> struct scpi_args_funcs<int64_t>
{
    constexpr static scpi_bool_t (*recv)(scpi_t *,
                                         int64_t *,
                                         scpi_bool_t) = SCPI_ParamInt64;
    constexpr static size_t (*send)(scpi_t *, int64_t val) = SCPI_ResultInt64;
};
template <> struct scpi_args_funcs<uint32_t>
{
    constexpr static scpi_bool_t (*recv)(scpi_t *,
                                         uint32_t *,
                                         scpi_bool_t) = SCPI_ParamInt32_Bool;
    constexpr static size_t (*send)(scpi_t *,
                                    uint32_t val) = SCPI_ResultInt32_Bool;
};
template <> struct scpi_args_funcs<double>
{
    constexpr static scpi_bool_t (*recv)(scpi_t *,
                                         double *,
                                         scpi_bool_t) = SCPI_ParamDouble;
    constexpr static size_t (*send)(scpi_t *, double val) = SCPI_ResultDouble;
};
template <> struct scpi_args_funcs<bool>
{
    // constexpr static scpi_bool_t (*recv)(scpi_t *, double *, scpi_bool_t) =
    // SCPI_ParamInt32;
    constexpr static size_t (*send)(scpi_t *, int32_t val) = SCPI_ResultInt32;
};
template <> struct scpi_args_funcs<DeviceStatus>
{
    // constexpr static scpi_bool_t (*recv)(scpi_t *, double *, scpi_bool_t) =
    // SCPI_ParamInt32;
    constexpr static size_t (*send)(scpi_t *, int32_t val) = SCPI_ResultInt32;
};

template <void (AbstractAxis::*mtd)()>
scpi_result_t SCPIFUNC_axisVoid(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN(get_axis(number)->*mtd)();
    TRY_END
    return SCPI_RES_OK;
}

template <int (AbstractAxis::*mtd)()>
scpi_result_t SCPIFUNC_axisVoidI(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN(get_axis(number)->*mtd)();
    TRY_END
    return SCPI_RES_OK;
}

template <typename Arg,
          void (AbstractAxis::*mtd)(Arg,
                                    const std::vector<std::pair<int, Arg>> &)>
scpi_result_t SCPIFUNC_axisInterpolationArg(scpi_t *context)
{
    //    bool sts;
    int number;
    int subax_total;

    std::vector<std::pair<int, Arg>> subax;
    Arg arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    if (!scpi_args_funcs<Arg>::recv(context, &arg, true))
        return SCPI_RES_ERR;
    if (!SCPI_ParamInt32(context, &subax_total, true))
        return SCPI_RES_ERR;

    for (int it = 0; it < subax_total; ++it)
    {
        int num;
        Arg arg;

        if (!SCPI_ParamInt32(context, &num, true))
            return SCPI_RES_ERR;
        ;
        if (!scpi_args_funcs<Arg>::recv(context, &arg, true))
            return SCPI_RES_ERR;

        PROTECT_AXIS_NUMBER(num);

        subax.push_back(std::make_pair(num, arg));
    }

    TRY_BEGIN(get_axis(number)->*mtd)(arg, subax);
    TRY_END
    return SCPI_RES_OK;
}

template <typename Ret, Ret (AbstractAxis::*mtd)()>
scpi_result_t SCPIFUNC_axisArgQ(scpi_t *context)
{
    int number;
    Ret ret;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    ret = (get_axis(number)->*mtd)();
    TRY_END_Q
    scpi_args_funcs<Ret>::send(context, ret);
    return SCPI_RES_OK;
}

template <typename Ret, Ret (AbstractAxis::*mtd)() const>
scpi_result_t SCPIFUNC_axisConstArgQ(scpi_t *context)
{
    int number;
    Ret ret;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    ret = (get_axis(number)->*mtd)();
    TRY_END_Q
    scpi_args_funcs<Ret>::send(context, ret);
    return SCPI_RES_OK;
}

template <void (Device::*mtd)()>
scpi_result_t SCPIFUNC_deviceVoid(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);
    TRY_BEGIN(devices_list[number]->*mtd)();
    TRY_END
    return SCPI_RES_OK;
}

template <typename Arg, void (AbstractAxis::*mtd)(Arg)>
scpi_result_t SCPIFUNC_axisArg(scpi_t *context)
{
    int number;
    Arg arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    if (!scpi_args_funcs<Arg>::recv(context, &arg, true))
        return SCPI_RES_ERR;
    TRY_BEGIN(get_axis(number)->*mtd)(arg);
    TRY_END
    return SCPI_RES_OK;
}

template <typename Arg, void (Device::*mtd)(Arg)>
scpi_result_t SCPIFUNC_deviceArg(scpi_t *context)
{
    int number;
    Arg arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);
    if (!scpi_args_funcs<Arg>::recv(context, &arg, true))
        return SCPI_RES_ERR;
    TRY_BEGIN(get_device(number)->*mtd)(arg);
    TRY_END
    return SCPI_RES_OK;
}

template <typename Ret, Ret (Device::*mtd)()>
scpi_result_t SCPIFUNC_deviceArgQ(scpi_t *context)
{
    int number;
    Ret ret;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);
    TRY_BEGIN
    ret = (devices_list[number]->*mtd)();
    TRY_END_Q
    scpi_args_funcs<Ret>::send(context, ret);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_SystemIPAddress(scpi_t *context)
{
    // Deprecated and removed
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_SystemStatusQ(scpi_t *context)
{
    SCPI_ResultInt32(context, System::instance()->checker.getStatus());
    return SCPI_RES_OK;
}

scpi_func_t SCPIFUNC_axisPreset = SCPIFUNC_axisVoid<&AbstractAxis::preset>;
scpi_func_t SCPIFUNC_axisGenerateTrigger =
    SCPIFUNC_axisVoid<&AbstractAxis::generate_trigger>;
scpi_func_t SCPIFUNC_axisManualTriggerMode =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::manual_trigger_mode>;
scpi_func_t SCPIFUNC_deviceGenerateTrigger =
    SCPIFUNC_deviceVoid<&Device::generate_trigger>;
scpi_func_t SCPIFUNC_deviceManualTriggerMode =
    SCPIFUNC_deviceArg<int32_t, &Device::manual_trigger_mode>;
scpi_func_t SCPIFUNC_axisScanNoTrigMode =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::no_trigger_mode>;
scpi_func_t SCPIFUNC_axisScanBiTrigMode =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::bitrigmode>;

scpi_func_t SCPIFUNC_devicePreset = SCPIFUNC_deviceVoid<&Device::preset>;

scpi_func_t SCPIFUNC_defaultSpeed_rpm =
    SCPIFUNC_axisArg<uint32_t, &AbstractAxis::defaultSpeed_rpm>;
scpi_func_t SCPIFUNC_defaultAccel_ms =
    SCPIFUNC_axisArg<uint32_t, &AbstractAxis::defaultAccel_ms>;
scpi_func_t SCPIFUNC_maxSpeed_rpm =
    SCPIFUNC_axisArg<uint32_t, &AbstractAxis::maxSpeed_rpm>;
scpi_func_t SCPIFUNC_minAccel_ms =
    SCPIFUNC_axisArg<uint32_t, &AbstractAxis::minAccel_ms>;

scpi_func_t SCPIFUNC_unitBackwardLimit =
    SCPIFUNC_axisArg<double, &AbstractAxis::unitBackwardLimit>;
scpi_func_t SCPIFUNC_unitForwardLimit =
    SCPIFUNC_axisArg<double, &AbstractAxis::unitForwardLimit>;

scpi_func_t SCPIFUNC_get_unitRatio =
    SCPIFUNC_axisArgQ<double, &AbstractAxis::unitRatio_forScpiApi>;
scpi_func_t SCPIFUNC_get_defaultSpeed_rpm =
    SCPIFUNC_axisConstArgQ<uint32_t, &AbstractAxis::defaultSpeed_rpm>;
scpi_func_t SCPIFUNC_get_defaultAccel_ms =
    SCPIFUNC_axisConstArgQ<uint32_t, &AbstractAxis::defaultAccel_ms>;
scpi_func_t SCPIFUNC_get_maxSpeed_rpm =
    SCPIFUNC_axisConstArgQ<uint32_t, &AbstractAxis::maxSpeed_rpm>;
scpi_func_t SCPIFUNC_get_minAccel_ms =
    SCPIFUNC_axisConstArgQ<uint32_t, &AbstractAxis::minAccel_ms>;
scpi_func_t SCPIFUNC_get_unitBackwardLimit =
    SCPIFUNC_axisConstArgQ<double, &AbstractAxis::unitBackwardLimit>;
scpi_func_t SCPIFUNC_get_unitForwardLimit =
    SCPIFUNC_axisConstArgQ<double, &AbstractAxis::unitForwardLimit>;

scpi_func_t SCPIFUNC_axisSpeed =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::setSpeed_rpm>;
scpi_func_t SCPIFUNC_axisSpeedQ =
    SCPIFUNC_axisConstArgQ<int32_t, &AbstractAxis::getSpeed_rpm>;
scpi_func_t SCPIFUNC_axisUnitSpeed =
    SCPIFUNC_axisArg<double, &AbstractAxis::setSpeed_ups>;
scpi_func_t SCPIFUNC_axisUnitSpeedQ =
    SCPIFUNC_axisConstArgQ<double, &AbstractAxis::getSpeed_ups>;
scpi_func_t SCPIFUNC_axisAccel =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::setAccel_ms>;
scpi_func_t SCPIFUNC_axisAccelQ =
    SCPIFUNC_axisConstArgQ<int32_t, &AbstractAxis::getAccel_ms>;
scpi_func_t SCPIFUNC_axisGainQ =
    SCPIFUNC_axisArgQ<double, &AbstractAxis::unitRatio_forScpiApi>;

scpi_func_t SCPIFUNC_axisUAccel =
    SCPIFUNC_axisArg<double, &AbstractAxis::setAccel_ups2>;
scpi_func_t SCPIFUNC_axisUAccelQ =
    SCPIFUNC_axisConstArgQ<double, &AbstractAxis::getAccel_ups2>;

// scpi_func_t SCPIFUNC_axisRelativeMove =
//     SCPIFUNC_axisArg<double, &AbstractAxis::relative_move>;
// scpi_func_t SCPIFUNC_axisAbsoluteMove =
//     SCPIFUNC_axisArg<double, &AbstractAxis::absolute_move>;
scpi_func_t SCPIFUNC_axisUnitRelativeMove =
    SCPIFUNC_axisArg<double, &AbstractAxis::relativeUnitMove>;
scpi_func_t SCPIFUNC_axisUnitAbsoluteMove =
    SCPIFUNC_axisArg<double, &AbstractAxis::absoluteUnitMove>;

scpi_func_t SCPIFUNC_deviceStop = SCPIFUNC_deviceVoid<&Device::stop>;
scpi_func_t SCPIFUNC_deviceRelativeMove =
    SCPIFUNC_deviceArg<double, &Device::relative_move>;
scpi_func_t SCPIFUNC_deviceAbsoluteMove =
    SCPIFUNC_deviceArg<double, &Device::absolute_move>;
scpi_func_t SCPIFUNC_deviceUnitRelativeMove =
    SCPIFUNC_deviceArg<double, &Device::relative_unit_move>;
scpi_func_t SCPIFUNC_deviceUnitAbsoluteMove =
    SCPIFUNC_deviceArg<double, &Device::absolute_unit_move>;

// scpi_func_t SCPIFUNC_axisInterpolationRelativeMove =
//     SCPIFUNC_axisInterpolationArg<int32_t,
//                                   &AbstractAxis::interpolationRelativeMove>;
// scpi_func_t SCPIFUNC_axisInterpolationAbsoluteMove =
//     SCPIFUNC_axisInterpolationArg<int32_t,
//   &AbstractAxis::interpolationAbsoluteMove>;
scpi_func_t SCPIFUNC_axisInterpolationUnitRelativeMove =
    SCPIFUNC_axisInterpolationArg<double,
                                  &AbstractAxis::interpolationRelativeUnitMove>;
scpi_func_t SCPIFUNC_axisInterpolationUnitAbsoluteMove =
    SCPIFUNC_axisInterpolationArg<double,
                                  &AbstractAxis::interpolationAbsoluteUnitMove>;

scpi_func_t SCPIFUNC_axisStop = SCPIFUNC_axisVoid<&AbstractAxis::stop>;
scpi_func_t SCPIFUNC_axisServoOn = SCPIFUNC_axisVoid<&AbstractAxis::servo_on>;
scpi_func_t SCPIFUNC_axisServoOff = SCPIFUNC_axisVoid<&AbstractAxis::servo_off>;

scpi_func_t SCPIFUNC_axisUnitPositionQ =
    SCPIFUNC_axisArgQ<double, &AbstractAxis::last_unit_position>;
scpi_func_t SCPIFUNC_axisUnitPositionNCorQ =
    SCPIFUNC_axisArgQ<double, &AbstractAxis::last_unit_position_not_corrected>;

scpi_func_t SCPIFUNC_devicePositionQ =
    SCPIFUNC_deviceArgQ<double, &Device::last_position>;
scpi_func_t SCPIFUNC_deviceUnitPositionQ =
    SCPIFUNC_deviceArgQ<double, &Device::last_unit_position>;

scpi_func_t SCPIFUNC_devTriggerPolarity =
    SCPIFUNC_deviceArg<int32_t, &Device::com_trigger_polarity>;
scpi_func_t SCPIFUNC_devTriggerDuration =
    SCPIFUNC_deviceArg<int32_t, &Device::com_trigger_duration>;

scpi_func_t SCPIFUNC_axisLimitSwitchStatusQ =
    SCPIFUNC_axisArgQ<int32_t, &AbstractAxis::getLimitSwitchStatus>;

scpi_func_t SCPIFUNC_deviceLimitSwitchStatusQ =
    SCPIFUNC_deviceArgQ<int32_t, &Device::getLimitSwitchStatus>;

scpi_func_t SCPIFUNC_axisOperationCodeQ =
    SCPIFUNC_axisArgQ<int32_t, &AbstractAxis::getOperationCode>;
scpi_func_t SCPIFUNC_axisStatusQ =
    SCPIFUNC_axisArgQ<int32_t, &AbstractAxis::getStatus>;
scpi_func_t SCPIFUNC_axisHaveScanQ =
    SCPIFUNC_axisArgQ<bool, &AbstractAxis::haveScan>;
scpi_func_t SCPIFUNC_axisHaveAbsoluteMoveQ =
    SCPIFUNC_axisArgQ<bool, &AbstractAxis::haveAbsoluteMove>;

scpi_func_t SCPIFUNC_axisHaveSetReferenceQ =
    SCPIFUNC_axisArgQ<bool, &AbstractAxis::haveSetReference>;

scpi_func_t SCPIFUNC_deviceStatusQ =
    SCPIFUNC_deviceArgQ<int32_t, &Device::getStatus_as_int32>;

scpi_func_t SCPIFUNC_deviceAlarmQ =
    SCPIFUNC_deviceArgQ<int32_t, &Device::getAlarmStatus>;

scpi_func_t SCPIFUNC_axisTrigReturnTime =
    SCPIFUNC_axisArgQ<int32_t, &AbstractAxis::trigreturn_time>;

scpi_func_t SCPIFUNC_axisTriggerPolarity =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::com_trigger_polarity>;
scpi_func_t SCPIFUNC_axisTriggerDuration =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::com_trigger_duration>;

scpi_result_t SCPIFUNC_axisSetZero(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    get_axis(number)->setNullPosition();
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisUseCorrectionQ(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    SCPI_ResultInt32(context, get_axis(number)->correction_table_used());
    TRY_END
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_deviceSetZero(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);
    TRY_BEGIN
    get_device(number)->set_zero_position();
    TRY_END
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisSetReference(scpi_t *context)
{
    SCPI_ErrorPush(context, SCPI_ERROR_TODO);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisSetUnitReference(scpi_t *context)
{
    SCPI_ErrorPush(context, SCPI_ERROR_TODO);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisJog(scpi_t *context)
{
    int number;
    int arg;

    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    SCPI_ParamInt32(context, &arg, true);

    if (arg != 1 && arg != -1)
        ILLEGAL_PARAMETR_VALUE_RETURN;

    TRY_BEGIN
    get_axis(number)->jog(arg == 1 ? FORWARD : BACKWARD);
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisIdentifierQ(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ResultMnemonic(context, get_axis(number)->name().c_str());

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_haveIntLazQ(scpi_t *context)
{
#if (HAVE_LAZER_PROXY)
    SCPI_ResultInt32(context, 1);
#else
    SCPI_ResultInt32(context, 0);
#endif
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisDevicesQ(scpi_t *context)
{
    int number;
    std::vector<int> devsNumbers;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    devsNumbers = get_axis(number)->deviceNumbers();
    TRY_END
    for (auto num : devsNumbers)
    {
        SCPI_ResultInt32(context, num);
    }
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisIsActiveQ(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);

    // SCPI_ResultInt32(context, get_axis(number)->isActive());
    SCPI_ResultInt32(context, 1); // Active mechanic removed
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_deviceIdentifierQ(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);

    SCPI_ResultMnemonic(context, devices_list[number]->name().c_str());

    return SCPI_RES_OK;
}

scpi_func_t SCPIFUNC_axisScanMove =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::scanMove>;
scpi_func_t SCPIFUNC_axisScanForwardZone =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::scanForwardZone>;
scpi_func_t SCPIFUNC_axisScanBackwardZone =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::scanBackwardZone>;

scpi_func_t SCPIFUNC_axisScanUnitMove =
    SCPIFUNC_axisArg<double, &AbstractAxis::scanUnitMove>;
scpi_func_t SCPIFUNC_axisScanUnitForwardZone =
    SCPIFUNC_axisArg<double, &AbstractAxis::scanUnitForwardZone>;
scpi_func_t SCPIFUNC_axisScanUnitBackwardZone =
    SCPIFUNC_axisArg<double, &AbstractAxis::scanUnitBackwardZone>;

scpi_func_t SCPIFUNC_axisScanMoveQ =
    SCPIFUNC_axisArgQ<int32_t, &AbstractAxis::scanMove>;
scpi_func_t SCPIFUNC_axisScanForwardZoneQ =
    SCPIFUNC_axisArgQ<int32_t, &AbstractAxis::scanForwardZone>;
scpi_func_t SCPIFUNC_axisScanBackwardZoneQ =
    SCPIFUNC_axisArgQ<int32_t, &AbstractAxis::scanBackwardZone>;

scpi_func_t SCPIFUNC_axisScanUnitMoveQ =
    SCPIFUNC_axisArgQ<double, &AbstractAxis::scanUnitMove>;
scpi_func_t SCPIFUNC_axisScanUnitForwardZoneQ =
    SCPIFUNC_axisArgQ<double, &AbstractAxis::scanUnitForwardZone>;
scpi_func_t SCPIFUNC_axisScanUnitBackwardZoneQ =
    SCPIFUNC_axisArgQ<double, &AbstractAxis::scanUnitBackwardZone>;

scpi_func_t SCPIFUNC_axisScanPoints =
    SCPIFUNC_axisArg<int32_t, &AbstractAxis::scanPoints>;
scpi_func_t SCPIFUNC_axisScanPointsQ =
    SCPIFUNC_axisArgQ<int32_t, &AbstractAxis::scanPoints>;
scpi_func_t SCPIFUNC_axisScanStart =
    SCPIFUNC_axisVoid<&AbstractAxis::scanStart>;
scpi_func_t SCPIFUNC_axisScanCompareStart =
    SCPIFUNC_axisVoid<&AbstractAxis::scanCompareStart>;
scpi_func_t SCPIFUNC_axisEnableExternalTriggerRecord =
    SCPIFUNC_axisVoid<&AbstractAxis::scanEnableExternalTriggerNotify>;

scpi_result_t SCPIFUNC_getScanApiVersionQ(scpi_t *context)
{
    // Deprecated: Захардкожена последняя версия платы, с которой использовалась
    // эта функция. Клиент использовал её для проверки работоспособности с
    // таблицей сканирования. Однако, объект оси совершенно не обязан знать, с
    // каким типом сканирующего оборудования он работает.
    SCPI_ResultInt32(context, 110);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_unitLimits(scpi_t *context)
{
    int number;
    double arg0;
    double arg1;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    if (!scpi_args_funcs<double>::recv(context, &arg0, true))
        return SCPI_RES_ERR;
    if (!scpi_args_funcs<double>::recv(context, &arg1, true))
        return SCPI_RES_ERR;

    TRY_BEGIN
    get_axis(number)->unitLimits(arg0, arg1);
    TRY_END
    return SCPI_RES_OK;
}

uint16_t progcounter = 0;
scpi_result_t SCPIFUNC_progTableRestart(scpi_t *context)
{
    int number;
    int32_t arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ParamInt32(context, &arg, true);

    progcounter = 0;

    TRY_BEGIN
    if (get_axis(number)->haveScan())
    {
        AxisSynchro *ax = (AxisSynchro *)get_axis(number);
        ax->synchro->eventctr_table_reset(arg);
    }
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisNoteQ(scpi_t *context)
{
    SCPI_ResultText(context, "deprecated function");
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_progTableRestartPrefix(scpi_t *context)
{
    int number;
    int32_t arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ParamInt32(context, &arg, true);

    progcounter = 0;

    TRY_BEGIN
    if (get_axis(number)->haveScan())
    {
        AxisSynchro *ax = (AxisSynchro *)get_axis(number);
        ax->synchro->eventctr_table_reset_prefix(arg);
    }
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_progTableRestartCycle(scpi_t *context)
{
    int number;
    int32_t arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ParamInt32(context, &arg, true);

    progcounter = 0;

    TRY_BEGIN
    if (get_axis(number)->haveScan())
    {
        AxisSynchro *ax = (AxisSynchro *)get_axis(number);
        ax->synchro->eventctr_table_reset_cycle(arg);
    }
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_progTableRestartPostfix(scpi_t *context)
{
    int number;
    int32_t arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ParamInt32(context, &arg, true);

    progcounter = 0;

    TRY_BEGIN
    if (get_axis(number)->haveScan())
    {
        AxisSynchro *ax = (AxisSynchro *)get_axis(number);
        ax->synchro->eventctr_table_reset_postfix(arg);
    }
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_progTableSetLoopsTotal(scpi_t *context)
{
    int number;
    int32_t arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ParamInt32(context, &arg, true);

    progcounter = 0;

    TRY_BEGIN
    if (get_axis(number)->haveScan())
    {
        AxisSynchro *ax = (AxisSynchro *)get_axis(number);
        ax->synchro->eventctr_set_loop_total(arg);
    }
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_progTablePush(scpi_t *context)
{
    int number;
    int32_t arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ParamInt32(context, &arg, true);

    TRY_BEGIN
    if (get_axis(number)->haveScan())
    {
        AxisSynchro *ax = (AxisSynchro *)get_axis(number);
        ax->synchro->eventctr_push_back(((progcounter & 0xFFFFFF) << 8) |
                                        (arg & 0xFF));
    }
    TRY_END

    progcounter++;

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_progSaveTable(scpi_t *context)
{
    int number;
    int32_t arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ParamInt32(context, &arg, true);

    TRY_BEGIN
    if (get_axis(number)->haveScan())
    {
        AxisSynchro *ax = (AxisSynchro *)get_axis(number);
        ax->synchro->eventctr_save_table(arg);
    }
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_progLoadTable(scpi_t *context)
{
    int number;
    int32_t arg;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ParamInt32(context, &arg, true);

    TRY_BEGIN
    if (get_axis(number)->haveScan())
    {
        AxisSynchro *ax = (AxisSynchro *)get_axis(number);
        ax->synchro->eventctr_load_table(arg);
    }
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisAlarmStringQ(scpi_t *context)
{
    int number;
    std::string alstr;

    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ResultText(context, get_axis(number)->status_string().c_str());

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_deviceAlarmStringQ(scpi_t *context)
{
    int number;
    std::string alstr;

    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    SCPI_ResultText(context, get_device(number)->getAlarm().c_str());

    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_VectorSpeed(scpi_t *context)
{
    int number;
    std::vector<double> spds;
    SCPI_CommandNumbers(context, &number, 1, -1);

    auto intgroup = find_group(number);

    bool success = true;
    while (success)
    {
        double arg;
        success = SCPI_ParamDouble(context, &arg, false);
        spds.push_back(arg);
    }

    intgroup->set_speeds_ups(spds);
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_CommonSpeed(scpi_t *context)
{
    int number;
    double spd;
    SCPI_CommandNumbers(context, &number, 1, -1);

    auto intgroup = find_group(number);
    SCPI_ParamDouble(context, &spd, true);

    intgroup->set_common_speed_ups(spd);
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_CommonAccel(scpi_t *context)
{
    int number;
    double acc;
    SCPI_CommandNumbers(context, &number, 1, -1);

    auto intgroup = find_group(number);
    SCPI_ParamDouble(context, &acc, true);

    intgroup->set_common_accel_ups2(acc);
    intgroup->set_common_decel_ups2(acc);
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_VectorAccel(scpi_t *context)
{
    int number;
    std::vector<double> accs;
    SCPI_CommandNumbers(context, &number, 1, -1);
    auto intgroup = find_group(number);
    bool success = true;
    while (success)
    {
        double arg;
        success = SCPI_ParamDouble(context, &arg, false);
        accs.push_back(arg);
    }
    intgroup->set_accels_ups(accs);
    intgroup->set_decels_ups(accs);
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_CommonSpeedQ(scpi_t *context)
{
    int number;
    double spd;
    SCPI_CommandNumbers(context, &number, 1, -1);

    auto intgroup = find_group(number);
    spd = intgroup->setted_speed_ups();
    SCPI_ResultDouble(context, spd);

    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_CommonAccelQ(scpi_t *context)
{
    int number;
    double acc;
    SCPI_CommandNumbers(context, &number, 1, -1);

    auto intgroup = find_group(number);
    acc = intgroup->setted_accel_ups2();
    SCPI_ResultDouble(context, acc);

    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_UIncMove(scpi_t *context)
{
    int number;
    std::vector<double> args;
    SCPI_CommandNumbers(context, &number, 1, -1);
    auto intgroup = find_group(number);
    auto dim = intgroup->dim();
    args.resize(dim);
    for (int i = 0; i < dim; i++)
    {
        bool success = SCPI_ParamDouble(context, &args[i], true);
        if (!success)
        {
            return SCPI_RES_ERR;
        }
    }
    TRY_BEGIN
    intgroup->incmove_command(ralgo::vecops::cast<std::vector<double>>(args));
    TRY_END;
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_UAbsMove(scpi_t *context)
{
    int number;
    std::vector<double> args;
    SCPI_CommandNumbers(context, &number, 1, -1);
    auto intgroup = find_group(number);
    auto dim = intgroup->dim();
    args.resize(dim);
    for (int i = 0; i < dim; i++)
    {
        bool success = SCPI_ParamDouble(context, &args[i], true);
        if (!success)
        {
            return SCPI_RES_ERR;
        }
    }
    TRY_BEGIN
    intgroup->absmove_command(ralgo::vecops::cast<std::vector<double>>(args));
    TRY_END;
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_UAbsMove_vec(scpi_t *context)
{
    int number;
    std::vector<std::vector<double>> args;
    SCPI_CommandNumbers(context, &number, 1, -1);
    auto intgroup = find_group(number);
    auto dim = intgroup->axis_dim();

    std::string command;
    command.resize(5096);
    size_t len = 0;

    bool success = SCPI_ParamCopyText(context, &command[0], 5096, &len, false);
    (void)success;

    command.resize(len);
    auto lst = nos::split(command, ',');

    size_t count_of_point = lst.size() / dim;
    args.resize(count_of_point);
    nos::println("Resize", count_of_point);
    for (auto &vec : args)
    {
        vec.resize(dim);
        nos::println("Resize dim", dim);
    }

    for (size_t idx = 0; idx < lst.size(); idx++)
    {
        size_t point_no = idx / dim;
        size_t point_dim = idx % dim;
        nos::println("Add subpoint", point_no, point_dim);
        args[point_no][point_dim] = std::stod(lst[idx]);
    }

    TRY_BEGIN

    intgroup->absmove_command(args);

    TRY_END;
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_stop(scpi_t *context)
{
    int number;
    std::vector<double> args;
    SCPI_CommandNumbers(context, &number, 1, -1);
    auto intgroup = find_group(number);
    TRY_BEGIN
    intgroup->stop();
    TRY_END;
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_dimensionQ(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    auto intgroup = find_group(number);
    auto dim = intgroup->dim();
    SCPI_ResultInt32(context, dim);
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_UAbsMove_Partial(scpi_t *context)
{
    int number;
    std::vector<std::pair<int, double>> args;
    SCPI_CommandNumbers(context, &number, 1, -1);

    auto intgroup = find_group(number);

    size_t len;
    bool success;
    char buf[4000];

    success = SCPI_ParamCopyText(context, buf, 4000, &len, false);
    buf[len] = 0;

    if (!success)
    {
        return SCPI_RES_ERR;
    }

    auto spl = nos::split(std::string(buf, len), ',');

    for (auto s : spl)
    {
        auto pair = nos::split(s, ':');
        if (pair.size() != 2)
        {
            return SCPI_RES_ERR;
        }

        std::string arg0 = std::string(nos::trim(pair[0]));
        std::string arg1 = std::string(nos::trim(pair[1]));

        auto num = strtol(arg0.c_str(), nullptr, 10);
        auto dat = std::stod(arg1);
        args.push_back({num, dat});
    }

    intgroup->absmove_parted(args);
    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_axes_UAbsMove_Partial(scpi_t *context)
{
    nos::println("Axes_UAbsMove_Partial!!!!!!!!!!!!!!!");

    int number;
    std::vector<std::pair<int, double>> args;
    SCPI_CommandNumbers(context, &number, 1, -1);

    // auto intgroup = find_group(number);

    size_t len;
    bool success;
    char buf[4000];

    success = SCPI_ParamCopyText(context, buf, 4000, &len, false);
    buf[len] = 0;

    if (!success)
    {
        return SCPI_RES_ERR;
    }

    auto spl = nos::split(std::string(buf, len), ',');

    for (auto s : spl)
    {
        auto pair = nos::split(s, ':');
        if (pair.size() != 2)
        {
            return SCPI_RES_ERR;
        }

        std::string arg0 = std::string(nos::trim(pair[0]));
        std::string arg1 = std::string(nos::trim(pair[1]));

        auto num = strtol(arg0.c_str(), nullptr, 10);
        auto dat = std::stod(arg1);
        args.push_back({num, dat});
    }

    TRY_BEGIN
    GlobalAbsmoveParted(args);
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_get_axesQ(scpi_t *context)
{
    int number;
    std::vector<double> args;
    SCPI_CommandNumbers(context, &number, 1, -1);

    auto intgroup = find_group(number);
    auto vec = intgroup->get_axes_indexes();

    for (auto i : vec)
    {
        SCPI_ResultInt32(context, i);
    }

    return SCPI_RES_OK;
}

scpi_result_t SPIFUNC_intgroup_idn(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);

    auto intgroup = find_group(number);
    SCPI_ResultMnemonic(context, intgroup->name().c_str());

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_getEncoderPositionQ(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    TRY_BEGIN
    SCPI_ResultInt32(context, get_axis(number)->scan_get_encoder_position());
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_scan_outputs_state(scpi_t *context)
{
    int number, out_num, out_state;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    if (!SCPI_ParamInt32(context, &out_num, true))
        return SCPI_RES_ERR;

    if (!SCPI_ParamInt32(context, &out_state, true))
        return SCPI_RES_ERR;

    TRY_BEGIN
    int result =
        get_axis(number)->scan_discrete_output_set_state(out_num, out_state);
    SCPI_ResultInt32(context, result);
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_scan_outputs_unical(scpi_t *context)
{
    int number, out_num;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    if (!SCPI_ParamInt32(context, &out_num, true))
        return SCPI_RES_ERR;

    TRY_BEGIN
    int result = get_axis(number)->scan_discrete_output_set_unical(out_num);
    SCPI_ResultInt32(context, result);
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_scan_outputs_disable(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);

    TRY_BEGIN
    int result = get_axis(number)->scan_discrete_output_disable();
    SCPI_ResultInt32(context, result);
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_deviceTypeQ(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);

    TRY_BEGIN
    auto dev = get_device(number);
    SCPI_ResultMnemonic(context, dev->declared_type().c_str());
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisTypeQ(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);

    TRY_BEGIN
    auto ax = get_axis(number);
    SCPI_ResultMnemonic(context, ax->declared_type().c_str());
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_deviceTestAlarm(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);

    TRY_BEGIN
    auto dev = get_device(number);
    dev->testAlarm();
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_deviceParameterQ(scpi_t *context)
{
    int number;
    char mnemo[48];
    size_t len;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);

    SCPI_ParamCopyText(context, mnemo, 48, &len, true);

    TRY_BEGIN
    auto dev = get_device(number);

    auto pair = dev->get_parameter(mnemo);

    SCPI_ResultText(context, pair.first.c_str());
    SCPI_ResultText(context, pair.second.c_str());
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_deviceParameter(scpi_t *context)
{
    int number;
    char mnemo[48];
    char value[48];
    char format[48];
    size_t mnemo_len;
    size_t value_len;
    size_t format_len;

    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);

    SCPI_ParamCopyText(context, mnemo, 48, &mnemo_len, true);
    SCPI_ParamCopyText(context, value, 48, &value_len, true);
    SCPI_ParamCopyText(context, format, 48, &format_len, true);

    TRY_BEGIN
    auto dev = get_device(number);
    dev->set_parameter(mnemo, value, format);
    TRY_END

    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_deviceTorqueQ(scpi_t *context)
{
    int number;
    double result = 0;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);

    TRY_BEGIN
    auto dev = get_device(number);
    result = dev->current_torque();
    TRY_END

    SCPI_ResultDouble(context, result);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_AssemblyQ(scpi_t *context)
{
    SCPI_ResultText(context, __DATE__ " " __TIME__);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_ScriptExecute(scpi_t *context)
{
    size_t len;
    int sts;
    char script[48];

    SCPI_ParamCopyText(context, script, 48, &len, true);

    nos::println("request script execution : ", std::string(script, len));
    sts = script_execution({script, len});

    if (sts < 0)
    {
        SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
        return SCPI_RES_ERR;
    }

    return SCPI_RES_OK;
}

#if HAVE_LAZER_PROXY
#include <deprecated/lazerproxy.h>
scpi_result_t SCPIFUNC_rangefinder_test(scpi_t *context)
{
    auto result = rangefinder.rpc_do_measure_test();
    SCPI_ResultDouble(context, result);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_rangefinder_bind(scpi_t *context)
{
    int num;

    if (!SCPI_ParamInt32(context, &num, true))
        return SCPI_RES_ERR;

    rangefinder.bind(num);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_rangefinder_unbind(scpi_t *context)
{
    rangefinder.unbind();
    return SCPI_RES_OK;
}
#else
scpi_result_t SCPIFUNC_rangefinder_test(scpi_t *context)
{
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_rangefinder_bind(scpi_t *context)
{
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_rangefinder_unbind(scpi_t *context)
{
    return SCPI_RES_OK;
}
#endif

scpi_result_t SCPIFUNC_axis_compare_table_enable(scpi_t *context)
{
    int number;
    bool en;
    SCPI_CommandNumbers(context, &number, 1, -1);
    SCPI_ParamBool(context, &en, true);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    get_axis(number)->compare_table_enable(en);
    TRY_END
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axis_compare_table_add_point(scpi_t *context)
{
    int number;
    double value;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    SCPI_ParamDouble(context, &value, true);
    TRY_BEGIN
    get_axis(number)->compare_table_add_point(value);
    TRY_END
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axis_compare_table_clear(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    get_axis(number)->compare_table_clear();
    TRY_END
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axis_scan_subsystem_reset(scpi_t *context)
{
    int number;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    get_axis(number)->scan_subsystem_reset();
    TRY_END
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisTandemSynchronizedQ(scpi_t *context)
{
    int number;
    bool result;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    CommonAxis &ax = dynamic_cast<CommonAxis &>(*get_axis(number));
    CNCTandemMovingClient &api =
        dynamic_cast<CNCTandemMovingClient &>(*ax.move_api());
    result = api.is_tandem_synchronized();
    TRY_END
    SCPI_ResultBool(context, result);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_servoHasLimitSwitchBackQ(scpi_t *context)
{
    int number;
    bool result;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);
    TRY_BEGIN
    AbstractServo *servo = dynamic_cast<AbstractServo *>(get_device(number));
    result = servo->has_limit_switch_back();
    TRY_END
    SCPI_ResultBool(context, result);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_servoHasLimitSwitchForwQ(scpi_t *context)
{
    int number;
    bool result;
    SCPI_CommandNumbers(context, &number, 1, -1);
    PROTECT_DEVICE_NUMBER(number);
    TRY_BEGIN
    AbstractServo *servo = dynamic_cast<AbstractServo *>(get_device(number));
    result = servo->has_limit_switch_forw();
    TRY_END
    SCPI_ResultBool(context, result);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisRelativeMove(scpi_t *context)
{
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisAbsoluteMove(scpi_t *context)
{
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisPositionQ(scpi_t *context)
{
    SCPI_ResultInt(context, 0);
    return SCPI_RES_OK;
}

scpi_result_t SCPIFUNC_axisScanGetLastPosition(scpi_t *context)
{
    int number;
    int offset;
    int size;
    int order;

    const scpi_choice_def_t options[] = {
        {"FROM_START", 0}, {"FROM_END", 1}, SCPI_CHOICE_LIST_END};

    SCPI_CommandNumbers(context, &number, 1, -1);

    SCPI_ParamInt(context, &offset, true);
    SCPI_ParamInt(context, &size, true);
    SCPI_ParamChoice(context, options, &order, true);

    std::vector<double> vec;
    PROTECT_AXIS_NUMBER(number);
    TRY_BEGIN
    vec = get_axis(number)->scan_points_position(offset, size, (bool)order);
    TRY_END

    nos::println("vec size = ", vec.size());
    nos::println("vec = ", vec);

    for (auto a : vec)
    {
        SCPI_ResultFloat(context, a);
    }
    return SCPI_RES_OK;
}
