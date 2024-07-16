#pragma once

#define HMI_SLAVE_ADDR 1

#define LED_GREEN 0
#define LED_RED 1

// COILS
#define A1_COIL 1
#define A2_COIL 2
#define A3_COIL 3
#define A4_COIL 4
#define A5_COIL 5
#define A6_COIL 6
#define STOP_COIL 7
#define RUN_COIL 8
#define HAND0_COIL 11 // 9
#define HAND1_COIL 12 // 10
#define DEC_COIL 9    // 11
#define INC_COIL 10   // 12
#define JOG_00_1_COIL 13
#define JOG_0_1_COIL 14
#define JOG_1_COIL 15
#define JOG_10_COIL 16
#define JOG_100_COIL 17
#define JOG_DIR_COIL 18
#define CURR_MODE_COIL 19
#define JOG_000_1_COIL 20
#define MOVE_TO_ZERO 21
#define LED1_COIL 9413
#define LED2_COIL 9414

// REGISTERS
#define GAUGE_LEFT_ADDR 5
#define GAUGE_LEFT_MIN_ADDR 10
#define GAUGE_LEFT_MAX_ADDR 12
#define GAUGE_RIGHT_MIN_ADDR 14
#define GAUGE_RIGHT_MAX_ADDR 16
#define SLIDER_SPEED_ADDR 9
#define CURR_AXIS_ADDR 20
#define LABEL_TARGET_POS_ADDR 22
#define CURRENT_POS 24
#define SLIDER_POS_ADDR 7
