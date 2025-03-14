/**
    @file
    Таблица SCPI комманд
*/

#include <comm/ncpi.h>
#include <comm/notifymap.h>
#include <comm/scpi-def.h>
#include <comm/scpi-func.h>
#include <scpi/scpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SyncScanApiClient.h>

const scpi_command_t scpi_commands[] = {
    //////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////// IEEE Mandated Commands (SCPI std V1999.0 4.1.1)
    /////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////
    {
        "*CLS",
        SCPI_CoreCls,
    },
    {
        "*ESE",
        SCPI_CoreEse,
    },
    {
        "*ESE?",
        SCPI_CoreEseQ,
    },
    {
        "*ESR?",
        SCPI_CoreEsrQ,
    },
    {
        "*IDN?",
        SCPI_CoreIdnQ,
    },

    {
        "*OPC",
        SCPI_CoreOpc,
    },
    {
        "*OPC?",
        SCPI_CoreOpcQ,
    },
    {
        "*RST",
        SCPI_CoreRst,
    },
    {
        "*SRE",
        SCPI_CoreSre,
    },
    {
        "*SRE?",
        SCPI_CoreSreQ,
    },
    {
        "*STB?",
        SCPI_CoreStbQ,
    },
    {
        "*WAI",
        SCPI_CoreWai,
    },

    //////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////// Required SCPI commands (SCPI std V1999.0 4.2.1)
    ///////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////
    {
        "SYSTem:ERRor[:NEXT]?",
        SCPI_SystemErrorNextQ,
    },
    {
        "SYSTem:ERRor:COUNt?",
        SCPI_SystemErrorCountQ,
    },
    {
        "SYSTem:VERSion?",
        SCPI_SystemVersionQ,
    },

    {
        "SYSTem:EXECSCRIPT",
        SCPIFUNC_ScriptExecute,
    },

    {
        "SYSTem:ASSEMBLY?",
        SCPIFUNC_AssemblyQ,
    },

    // POSITIONER API
    {
        "SYSTem:PRESet",
        SCPIFUNC_SystemPreset,
    },
    {
        "SYSTem:IPADDR",
        SCPIFUNC_SystemIPAddress, // удалён
    },
    {
        "SYSTem:STATus?",
        SCPIFUNC_SystemStatusQ,
    },
    {
        "SYSTem:STOP",
        SCPIFUNC_SystemStop,
    },
    {
        "SYSTem:POWOFF",
        SCPIFUNC_SystemPowerOff,
    },

    {
        "COMPat:INTLAZ?",
        SCPIFUNC_haveIntLazQ, // deprecated
    },

    ////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// AXIS'S API
    //////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    {
        "SYSTem:AXESTOTal?",
        SCPIFUNC_totalAxesQ,
    },
    {
        "SYSTem:IGRPTOTal?",
        SCPIFUNC_totalIntGroupsQ,
    },
    {
        "SYSTem:IGRPSTOTal?",
        SCPIFUNC_totalIntGroupsQ,
    },
    {
        "AXIS#:PRESET",
        SCPIFUNC_axisPreset,
    },
    {"AXES:UMOVe:ABSolute", SPIFUNC_axes_UAbsMove_Partial},
    {
        "AXIS#:TRIGGER",
        SCPIFUNC_axisGenerateTrigger,
    },
    {
        "AXIS#:MANTRIGmode",
        SCPIFUNC_axisManualTriggerMode,
    },
    {
        "DEV#:TRIGGER",
        SCPIFUNC_deviceGenerateTrigger,
    },
    {
        "DEV#:MANTRIGmode",
        SCPIFUNC_deviceManualTriggerMode,
    },
    {
        "AXIS#:TRIGRETTIME?",
        SCPIFUNC_axisTrigReturnTime,
    },

    {
        "AXIS#:SPEeed",
        SCPIFUNC_axisSpeed,
    },
    {
        "AXIS#:SPEeed?",
        SCPIFUNC_axisSpeedQ,
    },
    {
        "AXIS#:USPEeed",
        SCPIFUNC_axisUnitSpeed,
    },
    {
        "AXIS#:USPEeed?",
        SCPIFUNC_axisUnitSpeedQ,
    },
    {
        "AXIS#:ACCel",
        SCPIFUNC_axisAccel,
    },
    {
        "AXIS#:ACCel?",
        SCPIFUNC_axisAccelQ,
    },
    {
        "AXIS#:UACCel",
        SCPIFUNC_axisUAccel,
    },
    {
        "AXIS#:UACCel?",
        SCPIFUNC_axisUAccelQ,
    },
    {
        "AXIS#:GAIN?",
        SCPIFUNC_axisGainQ,
    },

    {
        "AXIS#:MOVe[:RELative]",
        SCPIFUNC_axisRelativeMove,
    },
    {
        "AXIS#:MOVe:ABSolute",
        SCPIFUNC_axisAbsoluteMove,
    },
    {
        "AXIS#:UMOVe[:RELative]",
        SCPIFUNC_axisUnitRelativeMove,
    },
    {
        "AXIS#:UMOVe:ABSolute",
        SCPIFUNC_axisUnitAbsoluteMove,
    },

    {
        "DEV#:MOVe[:RELative]",
        SCPIFUNC_deviceRelativeMove,
    },
    {
        "DEV#:MOVe:ABSolute",
        SCPIFUNC_deviceAbsoluteMove,
    },
    {
        "DEV#:UMOVe[:RELative]",
        SCPIFUNC_deviceUnitRelativeMove,
    },
    {
        "DEV#:UMOVe:ABSolute",
        SCPIFUNC_deviceUnitAbsoluteMove,
    },
    {
        "DEV#:STOP",
        SCPIFUNC_deviceStop,
    },

    // {
    //     "AXIS#:INTERP:MOVe[:RELative]",
    //     SCPIFUNC_axisInterpolationRelativeMove,
    // },
    // {
    //     "AXIS#:INTERP:MOVe:ABSolute",
    //     SCPIFUNC_axisInterpolationAbsoluteMove,
    // },
    {
        "AXIS#:INTERP:UMOVe[:RELative]",
        SCPIFUNC_axisInterpolationUnitRelativeMove,
    },
    {
        "AXIS#:INTERP:UMOVe:ABSolute",
        SCPIFUNC_axisInterpolationUnitAbsoluteMove,
    },

    {
        "AXIS#:JOG",
        SCPIFUNC_axisJog,
    },
    {
        "AXIS#:STOP",
        SCPIFUNC_axisStop,
    },
    {
        "AXIS#:SON",
        SCPIFUNC_axisServoOn,
    },
    {
        "AXIS#:SOFF",
        SCPIFUNC_axisServoOff,
    },
    {
        "AXIS#:SETTINGS:DEFSPEed",
        SCPIFUNC_defaultSpeed_rpm,
    },
    {
        "AXIS#:SETTINGS:DEFACCel",
        SCPIFUNC_defaultAccel_ms,
    },
    {
        "AXIS#:SETTINGS:MAXSPEed",
        SCPIFUNC_maxSpeed_rpm,
    },
    {
        "AXIS#:SETTINGS:MINAccel",
        SCPIFUNC_minAccel_ms,
    },
    {
        "AXIS#:SETTINGS:UBACKLIMit",
        SCPIFUNC_unitBackwardLimit,
    },
    {
        "AXIS#:SETTINGS:UFORWLIMit",
        SCPIFUNC_unitForwardLimit,
    },

    {
        "AXIS#:SETTINGS:ULIMITS",
        SCPIFUNC_unitLimits,
    },

    {
        "AXIS#:SETTINGS:RATIO?",
        SCPIFUNC_get_unitRatio,
    },
    {
        "AXIS#:SETTINGS:DEFSPEed?",
        SCPIFUNC_get_defaultSpeed_rpm,
    },
    {
        "AXIS#:SETTINGS:DEFACCel?",
        SCPIFUNC_get_defaultAccel_ms,
    },
    {
        "AXIS#:SETTINGS:MAXSPEed?",
        SCPIFUNC_get_maxSpeed_rpm,
    },
    {
        "AXIS#:SETTINGS:MINAccel?",
        SCPIFUNC_get_minAccel_ms,
    },
    {
        "AXIS#:SETTINGS:UBACKLIMit?",
        SCPIFUNC_get_unitBackwardLimit,
    },
    {
        "AXIS#:SETTINGS:UFORWLIMit?",
        SCPIFUNC_get_unitForwardLimit,
    },

    {
        "AXIS#:SETZERo",
        SCPIFUNC_axisSetZero,
    },
    {
        "DEV#:SETZERo",
        SCPIFUNC_deviceSetZero,
    },
    {
        "AXIS#:STAT:ACTive?",
        SCPIFUNC_axisIsActiveQ,
    },
    {
        "AXIS#:SETREFerence",
        SCPIFUNC_axisSetReference,
    },
    {
        "AXIS#:SETUREFerence",
        SCPIFUNC_axisSetUnitReference,
    },
    {
        "AXIS#:STATus:IDN?",
        SCPIFUNC_axisIdentifierQ,
    },
    {
        "AXIS#:STATus:POSition?",
        SCPIFUNC_axisPositionQ,
    },
    {
        "AXIS#:STATus:TANDEM:SYNC?",
        SCPIFUNC_axisTandemSynchronizedQ,
    },
    {
        "AXIS#:STATus:UPOSition?",
        SCPIFUNC_axisUnitPositionQ,
    },
    {
        "AXIS#:POSition?",
        SCPIFUNC_axisPositionQ,
    },
    {
        "AXIS#:UPOSition?",
        SCPIFUNC_axisUnitPositionQ,
    },
    {
        "AXIS#:UPOSition:NCOR?",
        SCPIFUNC_axisUnitPositionNCorQ,
    },
    {
        "AXIS#:STATus:LSWItch?",
        SCPIFUNC_axisLimitSwitchStatusQ,
    },
    {
        "DEV#:LSWItch?",
        SCPIFUNC_deviceLimitSwitchStatusQ,
    },
    {
        "AXIS#:STATus:OPcode?",
        SCPIFUNC_axisOperationCodeQ,
    },
    {
        "AXIS#:STATus[:STATus]?",
        SCPIFUNC_axisStatusQ,
    },
    {
        "AXIS#:STATus:DEVS?",
        SCPIFUNC_axisDevicesQ,
    },
    {
        "AXIS#:NOTE?",
        SCPIFUNC_axisNoteQ, // Удалена
    },
    {
        "AXIS#:SETTINGS:USECORRECTION?",
        SCPIFUNC_axisUseCorrectionQ, // Сама работа с таблицей коррекции
        // вынесена в отдельный api
    },

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////AXIS_COMPAT/////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    {
        "AXIS#:COMPat:SCAN?",
        SCPIFUNC_axisHaveScanQ,
    },
    {
        "AXIS#:COMPat:ABSMove?",
        SCPIFUNC_axisHaveAbsoluteMoveQ,
    },
    {
        "DEV#:COMPat:LSWI:BACK?",
        SCPIFUNC_servoHasLimitSwitchBackQ,
    },
    {
        "DEV#:COMPat:LSWI:FORW?",
        SCPIFUNC_servoHasLimitSwitchForwQ,
    },
    {
        "AXIS#:COMPat:REFSet?",
        SCPIFUNC_axisHaveSetReferenceQ,
    },

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////AXIS_SCAN///////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    {
        "AXIS#:SCAN:MOVe",
        SCPIFUNC_axisScanMove,
    },
    {
        "AXIS#:SCAN:FWRDzone",
        SCPIFUNC_axisScanForwardZone,
    },
    {
        "AXIS#:SCAN:BWRDzone",
        SCPIFUNC_axisScanBackwardZone, // deprecated
    },
    {
        "AXIS#:SCAN:UMOVE",
        SCPIFUNC_axisScanUnitMove,
    },
    {
        "AXIS#:SCAN:UFWRDzone",
        SCPIFUNC_axisScanUnitForwardZone,
    },
    {
        "AXIS#:SCAN:UBWRDzone",
        SCPIFUNC_axisScanUnitBackwardZone, // deprecated
    },

    {
        "AXIS#:SCAN:START",
        SCPIFUNC_axisScanStart, // deprecated
    },
    {
        "AXIS#:SCAN:COMPSTART",
        SCPIFUNC_axisScanCompareStart,
    },
    {
        "AXIS#:SCAN:POINTS",
        SCPIFUNC_axisScanPoints,
    },
    {
        "AXIS#:SCAN:NOTRIGMODE",
        SCPIFUNC_axisScanNoTrigMode,
    },
    {
        "AXIS#:SCAN:BITRIGMODE",
        SCPIFUNC_axisScanBiTrigMode,
    },

    {
        "AXIS#:SCAN:EXTRECORD",
        SCPIFUNC_axisEnableExternalTriggerRecord, // хрен его знает, что это
                                                  // но оно похоже не
                                                  // используется Может иметь
                                                  // ценность для отладки
    },

    {
        "AXIS#:SCAN:TRIGDURATION",
        SCPIFUNC_axisTriggerDuration, // Длительность ванпульса генерируемого
        // платой
    },

    {
        "AXIS#:SCAN:TRIGPOLARITY",
        SCPIFUNC_axisTriggerPolarity, // Полярность ванпульса генерируемого
                                      // платой
    },

    {
        "AXIS#:SCAN:GETLASTPOS?",
        SCPIFUNC_axisScanGetLastPosition,
    },

    {
        "AXIS#:SCAN:MOVE?",
        SCPIFUNC_axisScanMoveQ,
    },
    {
        "AXIS#:SCAN:FWRDzone?",
        SCPIFUNC_axisScanForwardZoneQ,
    },
    {
        "AXIS#:SCAN:BWRDzone?",
        SCPIFUNC_axisScanBackwardZoneQ,
    },
    {
        "AXIS#:SCAN:UMOVE?",
        SCPIFUNC_axisScanUnitMoveQ,
    },
    {
        "AXIS#:SCAN:UFWRDzone?",
        SCPIFUNC_axisScanUnitForwardZoneQ,
    },
    {
        "AXIS#:SCAN:UBWRDzone?",
        SCPIFUNC_axisScanUnitBackwardZoneQ,
    },
    {
        "AXIS#:SCAN:GETENC?",
        SCPIFUNC_getEncoderPositionQ,
    },

    {"AXIS#:SCAN:COMPTBL:ENable", SCPIFUNC_axis_compare_table_enable},
    {"AXIS#:SCAN:COMPTBL:ADD", SCPIFUNC_axis_compare_table_add_point},
    {"AXIS#:SCAN:COMPTBL:CLEar", SCPIFUNC_axis_compare_table_clear},
    {"AXIS#:SCAN:RESET", SCPIFUNC_axis_scan_subsystem_reset},

    {
        "AXIS#:SCAN:VERSION?",
        SCPIFUNC_getScanApiVersionQ,
    },

    {"AXIS#:SCAN:PRGTBL:RESTART", SCPIFUNC_progTableRestart},
    {"AXIS#:SCAN:PRGTBL:RPREF", SCPIFUNC_progTableRestartPrefix},
    {"AXIS#:SCAN:PRGTBL:RCYCLE", SCPIFUNC_progTableRestartCycle},
    {"AXIS#:SCAN:PRGTBL:RPOST", SCPIFUNC_progTableRestartPostfix},
    {"AXIS#:SCAN:PRGTBL:LOOPS", SCPIFUNC_progTableSetLoopsTotal},
    {"AXIS#:SCAN:PRGTBL:PUSH", SCPIFUNC_progTablePush},
    {"AXIS#:SCAN:PRGTBL:SAVE", SCPIFUNC_progSaveTable},
    {"AXIS#:SCAN:PRGTBL:LOAD", SCPIFUNC_progLoadTable},

    // ручное управление состояниями выводов
    {"AXIS#:SCAN:OUTS:SET", SCPIFUNC_scan_outputs_state},
    {"AXIS#:SCAN:OUTS:UNICal", SCPIFUNC_scan_outputs_unical},
    {"AXIS#:SCAN:OUTS:DISable", SCPIFUNC_scan_outputs_disable},

    {
        "DEV#:TRIGDURATION",
        SCPIFUNC_devTriggerDuration,
    },

    {
        "DEV#:TRIGPOLARITY",
        SCPIFUNC_devTriggerPolarity,
    },

    /////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////IGROUPS/////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    {"IGRP#:IDN?", SPIFUNC_intgroup_idn},
    {"IGRP#:DIMension?", SPIFUNC_intgroup_dimensionQ},
    {"IGRP#:USPEed[:COMmon]", SPIFUNC_intgroup_CommonSpeed},
    {"IGRP#:USPEed:VECtor", SPIFUNC_intgroup_VectorSpeed},
    {"IGRP#:USPEed[:COMmon]?", SPIFUNC_intgroup_CommonSpeedQ},
    {"IGRP#:UACCel[:COMmon]", SPIFUNC_intgroup_CommonAccel},
    {"IGRP#:UACCel:VECtor", SPIFUNC_intgroup_VectorAccel},
    {"IGRP#:UACCel[:COMmon]?", SPIFUNC_intgroup_CommonAccelQ},
    {"IGRP#:UMOV", SPIFUNC_intgroup_UIncMove},
    {"IGRP#:STOP", SPIFUNC_intgroup_stop},
    {"IGRP#:UMOV:ABS", SPIFUNC_intgroup_UAbsMove},
    {"IGRP#:UMOV:ABS:VEC", SPIFUNC_intgroup_UAbsMove_vec},
    {"IGRP#:PUMOV:ABS", SPIFUNC_intgroup_UAbsMove_Partial},
    {"IGRP#:AXIDXS?", SPIFUNC_intgroup_get_axesQ},

    ////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////// DEVICE'S API
    //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    {
        "SYSTem:DEVSTOTal?",
        SCPIFUNC_totalDevicesQ,
    },
    {
        "DEV#:PRESET",
        SCPIFUNC_devicePreset,
    },

    {
        "DEV#:IDN?",
        SCPIFUNC_deviceIdentifierQ,
    },
    {
        "DEV#[:STATus]?",
        SCPIFUNC_deviceStatusQ,
    },
    {
        "DEV#:ALM?",
        SCPIFUNC_deviceAlarmQ,
    },

    {
        "DEV#:POSition?",
        SCPIFUNC_devicePositionQ,
    },
    {
        "DEV#:UPOSition?",
        SCPIFUNC_deviceUnitPositionQ,
    },

    {
        "DEV#:TESTALM",
        SCPIFUNC_deviceTestAlarm,
    },

    // Установка параметров
    {"DEV#:PARAM", SCPIFUNC_deviceParameter},
    {"DEV#:PARAM?", SCPIFUNC_deviceParameterQ},
    {"DEV#:TYPE?", SCPIFUNC_deviceTypeQ},

    {"DEV#:TORQUE?", SCPIFUNC_deviceTorqueQ},

    {
        "DEV#:ALMSTR?",
        SCPIFUNC_deviceAlarmStringQ,
    },

    {
        "AXIS#:ALMSTR?",
        SCPIFUNC_axisAlarmStringQ,
    },

    {"AXIS#:STAT:TYPE?", SCPIFUNC_axisTypeQ},

    //////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////// MEAS
    /////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////
    {"MEAS:LASER:TEST", SCPIFUNC_rangefinder_test},     // deprecated
    {"MEAS:LASER:BIND", SCPIFUNC_rangefinder_bind},     // deprecated
    {"MEAS:LASER:UNBIND", SCPIFUNC_rangefinder_unbind}, // deprecated

    SCPI_CMD_LIST_END};

#define NOTIFY_REGISTRY(name)                                                  \
    {CONCATE(NPattern_, name), NCPI_setNotification, CONCATE(Tag_, name)},     \
    {                                                                          \
        STRINGCONCATE(CONCATE(NPattern_, name), "?"), NCPI_notificationQ,      \
            CONCATE(Tag_, name)                                                \
    }

#define NOTIFY_REGISTRY_2(name)                                                \
    {CONCATE(NPattern_, name),                                                 \
     NCPI_setNotification_without,                                             \
     CONCATE(Tag_, name)},                                                     \
    {                                                                          \
        STRINGCONCATE(CONCATE(NPattern_, name), "?"), NCPI_notificationQ,      \
            CONCATE(Tag_, name)                                                \
    }

#define SMOOTHNOTIFY_REGISTRY(name)                                            \
    {CONCATE(NPattern_, name),                                                 \
     NCPI_setSmoothNotification,                                               \
     CONCATE(Tag_, name)},                                                     \
    {                                                                          \
        STRINGCONCATE(CONCATE(NPattern_, name), "?"), NCPI_notificationQ,      \
            CONCATE(Tag_, name)                                                \
    }

const scpi_command_t ncpi_commands[] = {
    {
        "*CLS",
        SCPI_CoreCls,
    },
    {
        "SYSTem:ERRor[:NEXT]?",
        SCPI_SystemErrorNextQ,
    },
    {
        "SYSTem:ERRor:COUNt?",
        SCPI_SystemErrorCountQ,
    },
    {
        "SYSTem:VERSion?",
        SCPI_SystemVersionQ,
    },
    {
        "NOT:UNSUBSCRIBE",
        NCPI_unsubscribe,
    },

    NOTIFY_REGISTRY(SystemStatus),
    NOTIFY_REGISTRY(DeviceStatus),
    NOTIFY_REGISTRY(AxisStatus),
    NOTIFY_REGISTRY(AxisOperationStatus),
    NOTIFY_REGISTRY(AxisOperationStopType),
    NOTIFY_REGISTRY(AxisTandemSynchronized),

    SMOOTHNOTIFY_REGISTRY(AxisUnitPosition),
    SMOOTHNOTIFY_REGISTRY(AxisUnitPositionNotCorrected),
    SMOOTHNOTIFY_REGISTRY(AxisPosition),

    NOTIFY_REGISTRY(AxisScanPoint),
    NOTIFY_REGISTRY(AxisScanTrigError),

    NOTIFY_REGISTRY(AxisLimitSwitch),
    NOTIFY_REGISTRY(AxisTorque),
    NOTIFY_REGISTRY(ServoTorque),
    SMOOTHNOTIFY_REGISTRY(ServoRawPosition),
    SMOOTHNOTIFY_REGISTRY(ServoUnitPosition),
    SMOOTHNOTIFY_REGISTRY(ServoNonCorrectedUnitPosition),
    NOTIFY_REGISTRY(TestTheme),
    NOTIFY_REGISTRY_2(MessageBus),
    NOTIFY_REGISTRY_2(MessageChannel),
    NOTIFY_REGISTRY(ServoLimitSwitch),
    SCPI_CMD_LIST_END};

scpi_interface_t scpi_interface = {
    SCPI_Error,
    SCPI_Write,
    SCPI_Control,
    SCPI_Flush,
    SCPI_Reset,
};

scpi_interface_t ncpi_interface = {
    NCPI_Error,
    NCPI_Write,
    NCPI_Control,
    NCPI_Flush,
    NCPI_Reset,
};

scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];
