// HAL module for controlling the joystick on BeagleBoard

#include "hal/joystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdbool.h>

static int i2c_file_desc = -1;
static bool joystick_initialized = false;

static int init_i2c_bus(char* bus, int address)
{
	int i2c_file_desc = open(bus, O_RDWR);
	if (i2c_file_desc == -1) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(EXIT_FAILURE);
	}

	if (ioctl(i2c_file_desc, I2C_SLAVE, address) == -1) {
		perror("Unable to set I2C device to slave address.");
		exit(EXIT_FAILURE);
	}
	return i2c_file_desc;
}

static void write_i2c_reg16(int i2c_file_desc, uint8_t reg_addr, uint16_t value)
{
	int tx_size = 1 + sizeof(value);
	uint8_t buff[tx_size];
	buff[0] = reg_addr;
	buff[1] = (value & 0xFF);
	buff[2] = (value & 0xFF00) >> 8;
	int bytes_written = write(i2c_file_desc, buff, tx_size);
	if (bytes_written != tx_size) {
		perror("Unable to write i2c register");
		exit(EXIT_FAILURE);
	}
}

static uint16_t read_i2c_reg16(int i2c_file_desc, uint8_t reg_addr)
{
	// To read a register, must first write the address
	int bytes_written = write(i2c_file_desc, &reg_addr, sizeof(reg_addr));
	if (bytes_written != sizeof(reg_addr)) {
		perror("Unable to write i2c register.");
		exit(EXIT_FAILURE);
	}

	// Now read the value and return it
	uint16_t value = 0;
	int bytes_read = read(i2c_file_desc, &value, sizeof(value));
	if (bytes_read != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(EXIT_FAILURE);
	}

	// Convert byte order and shift bits into place
    uint16_t new_value = ((value & 0xFF) << 8) | ((value & 0xFF00) >> 8);
	return new_value >> 4;
}

void joystick_init(void) {
	i2c_file_desc = init_i2c_bus(I2CDRV_LINUX_BUS, I2C_DEVICE_ADDRESS);
	joystick_initialized = true;
}

void joystick_cleanup(void) {
	if(!joystick_initialized) return;

	close(i2c_file_desc);
	i2c_file_desc = -1;
	joystick_initialized = false;
}

static uint16_t read_joystick_position(uint16_t value) {
	write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, value);
    usleep(5000); // 5ms
    return read_i2c_reg16(i2c_file_desc, REG_DATA);
}

// Taken from (https://stackoverflow.com/a/59320692)
static float normalize_dir(int value, int min, int max) {
	return ((float)(value - min) / (max-min)) * 100.0f;
}

JoystickDirection joystick_get_direction(void) {
	if(!joystick_initialized) return JOYSTICK_NONE;

	// Read X and Y axis
	int y_axis = read_joystick_position(TLA2024_CHANNEL_Y_CONF_1);
    int x_axis = read_joystick_position(TLA2024_CHANNEL_X_CONF_0);

	float x_percent = normalize_dir(x_axis, JOYSTICK_X_MAX_LEFT, JOYSTICK_X_MAX_RIGHT);
    float y_percent = normalize_dir(y_axis, JOYSTICK_Y_MAX_UP, JOYSTICK_Y_MAX_DOWN);

	// Calculate which direction
	if (y_percent >= 80.0f) return JOYSTICK_DOWN;
    if (y_percent <= 20.0f) return JOYSTICK_UP;
    if (x_percent <= 20.0f) return JOYSTICK_LEFT;
    if (x_percent >= 80.0f) return JOYSTICK_RIGHT;


    return JOYSTICK_NONE;
}