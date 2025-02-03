#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdint.h>

// Device bus & address
#define I2CDRV_LINUX_BUS "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x48

// Register addresses in TLA2024
#define REG_CONFIGURATION 0x01
#define REG_DATA 0x00

// Configuration register values for different channels
#define TLA2024_CHANNEL_X_CONF_0 0x83D2
#define TLA2024_CHANNEL_Y_CONF_1 0x83C2
#define TLA2024_CHANNEL_CONF_2 0x83E2
#define TLA2024_CHANNEL_CONF_3 0x83F2

// Find the X and Y axis min and max values from joystick testing
#define JOYSTICK_Y_NEUTRAL 839
#define JOYSTICK_Y_MAX_UP 30
#define JOYSTICK_Y_MAX_DOWN 1643

#define JOYSTICK_X_NEUTRAL 805
#define JOYSTICK_X_MAX_LEFT 0
#define JOYSTICK_X_MAX_RIGHT 1641

typedef enum {
    JOYSTICK_NONE,
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT
} JoystickDirection;

void joystick_init(void);
void joystick_cleanup(void);
JoystickDirection joystick_get_direction(void);

#endif