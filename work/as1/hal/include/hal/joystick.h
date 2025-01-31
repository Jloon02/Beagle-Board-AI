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
#define TLA2024_CHANNEL_CONF_0 0x83C2
#define TLA2024_CHANNEL_CONF_1 0x83D2
#define TLA2024_CHANNEL_CONF_2 0x83E2
#define TLA2024_CHANNEL_CONF_3 0x83F2

// Default selected channel
#define SELECTED_CHANNEL_CONF TLA2024_CHANNEL_CONF_0


static int init_i2c_bus(char* bus, int address);
static void write_i2c_reg16(int i2c_file_desc, uint8_t reg_addr, uint16_t value);
static uint16_t read_i2c_reg16(int i2c_file_desc, uint8_t reg_addr);

#endif