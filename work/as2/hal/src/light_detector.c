// light_detector.c
// implementation for HAL light sensor with ADC through the I2C

#include "hal/light_detector.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

//#define _POSIX_C_SOURCE 199309L // For clock time

static int i2c_file_desc = -1;
static bool detectorInitialized = false;
static bool isThreadRunning = false;
static pthread_t detectorThread;
static pthread_mutex_t detectorMutex = PTHREAD_MUTEX_INITIALIZER;
static uint16_t lightValue;

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

static void* lightDetectorThreadFunc(void *none) {
	assert(detectorInitialized);
	(void)none; // Suppress unused parameter warning

	while(isThreadRunning) {
		// Read from the Light detector
		write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, TLA2024_CHANNEL_LIGHT);
		usleep(100); // delay needed between writes

		pthread_mutex_lock(&detectorMutex);
		lightValue = read_i2c_reg16(i2c_file_desc, REG_DATA);
		pthread_mutex_unlock(&detectorMutex);
	}

	return NULL;
}

void LightDetector_init(void) {
	assert(!detectorInitialized);

	i2c_file_desc = init_i2c_bus(I2CDRV_LINUX_BUS, I2C_DEVICE_ADDRESS);
	isThreadRunning = true;

	// Thread to continuously read light
	if (pthread_create(&detectorThread, NULL, lightDetectorThreadFunc, NULL) != 0) {
        perror("Error on creating light detector thread");
        exit(EXIT_FAILURE);
    }

	detectorInitialized = true;
}

void LightDetector_cleanup(void) {
	assert(detectorInitialized);

	isThreadRunning = false;
	i2c_file_desc = -1;

	close(i2c_file_desc);
	pthread_cancel(detectorThread);
	pthread_join(detectorThread, NULL);

	pthread_mutex_destroy(&detectorMutex);
	detectorInitialized = false;
}

float LightDetector_readVoltage(void) {
	float value;

	// Get instant of current light value
	pthread_mutex_lock(&detectorMutex);
	value = lightValue;
	pthread_mutex_unlock(&detectorMutex);

	// Analog ADC to voltage convertion
	return value * (ADC_VRED / ADC_MAX);
}