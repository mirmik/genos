/**
    @file
    Паттерны комманд системы NCPI
*/
#ifndef NOTIFY_MAP_H
#define NOTIFY_MAP_H

#include "comm/ncpi.h"

#define STRINGCONCATE(a, b) a b
#define CONCATE(a, b) a##b

#define NPattern_AxisOperationStatus "NOT:AXIS#:OPSTATus"
#define NPattern_AxisOperationStopType "NOT:AXIS#:OPSTOPtype"
#define NPattern_TestTheme "NOT:TEST"
#define NPattern_SystemStatus "NOT:SYSTem:STATus"
#define NPattern_AxisStatus "NOT:AXIS#:STATus"
#define NPattern_AxisTandemSynchronized "NOT:AXIS#:STATus:TANDEM:SYNC"
#define NPattern_AxisPosition "NOT:AXIS#:POSition"
#define NPattern_AxisUnitPosition "NOT:AXIS#:UPOSition"
#define NPattern_AxisUnitPositionNotCorrected "NOT:AXIS#:UPOSition:NCOR"
#define NPattern_DeviceStatus "NOT:DEVice#:STATus"
#define NPattern_DeviceAlarm "NOT:DEVice#:ALARM"
#define NPattern_DeviceReady "NOT:DEVice#:READY"
#define NPattern_AxisScanPoint "NOT:AXIS#:SCAN:POINT"
#define NPattern_AxisScanFinish "NOT:AXIS#:SCAN:FINISH"
#define NPattern_AxisScanTrigError "NOT:AXIS#:SCAN:TRIGERROR"
#define NPattern_AxisLimitSwitch "NOT:AXIS#:LSWItch"
#define NPattern_AxisTorque "NOT:AXIS#:TORQ"
#define NPattern_ServoTorque "NOT:DEV#:TORQ"
#define NPattern_ServoUnitPosition "NOT:DEV#:UPOS"
#define NPattern_ServoNonCorrectedUnitPosition "NOT:DEV#:NCORUPOS"
#define NPattern_ServoRawPosition "NOT:DEV#:POS"
#define NPattern_MessageBus "NOT:SYST:MSGBUS"
#define NPattern_MessageChannel "NOT:SYST:MSGCHAN"
#define NPattern_ServoLimitSwitch "NOT:DEV#:LSWItch"

#define Tag_AxisInfoSpeed 0
#define Tag_AxisInfoPosition 1
#define Tag_AxisOperationStatus 3
#define Tag_TestTheme 5
#define Tag_SystemStatus 6
#define Tag_AxisStatus 8
#define Tag_DeviceStatus 10
#define Tag_AxisPosition 12
#define Tag_AxisUnitPosition 13
#define Tag_AxisScanPoint 14
#define Tag_AxisScanTrigError 15
#define Tag_AxisOperationStopType 16
#define Tag_AxisLimitSwitch 17
#define Tag_AxisTorque 18
#define Tag_DeviceAlarm 19
#define Tag_DeviceReady 20
#define Tag_MessageBus 21
#define Tag_ServoTorque 22
#define Tag_MessageChannel 23
#define Tag_ServoUnitPosition 24
#define Tag_ServoRawPosition 25
#define Tag_ServoLimitSwitch 26
#define Tag_AxisTandemSynchronized 27
#define Tag_AxisUnitPositionNotCorrected 28
#define Tag_ServoNonCorrectedUnitPosition 29
#define Tag_AxisScanFinish 30

#endif
