// adcLightSensor.h
// Detect light senor via ADC over the I2C
// These set of methods provides ADC value

#ifndef _LIGHTDETECTOR_H_
#define _LIGHTDETECTOR_H_

// Device bus & address
#define I2CDRV_LINUX_BUS "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x48

// Register addresses in TLA2024
#define REG_CONFIGURATION 0x01
#define REG_DATA 0x00

// Configuration register values for different channels
#define TLA2024_CHANNEL_LIGHT 0x83E2

// ADC values
#define ADC_MAX 4096.0 // 12 bit
#define ADC_VRED 3.3// voltage for beagleboard is 3.3V

// Read and get the current light level from dectector light sensor
float LightDetector_readVoltage(void);
void LightDetector_init(void);
void LightDetector_cleanup(void);

#endif