// Main program to build the application

#include "reaction_game.h"

#include "hal/button.h"
#include "hal/joystick.h"
#include "hal/LED.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void led_trigger()
{
    const char *led = "ACT";

    // Set LED to none (direct control)
    set_led_trigger(led, "none");

    // Blink LED 5 times
    for (int i = 0; i < 5; i++) {
        set_led_brightness(led, 1);  // Turn on
        sleep_for(0.5);             // Wait 0.5 seconds
        set_led_brightness(led, 0); // Turn off
        sleep_for(0.5);             // Wait 0.5 seconds
    }

    // Restore LED to none mode
    set_led_trigger(led, "none");
}

int main()
{
	printf("Read TLA2024 ADC\n");

	int i2c_file_desc = init_i2c_bus(I2CDRV_LINUX_BUS, I2C_DEVICE_ADDRESS);

	// Select the channel
	write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, SELECTED_CHANNEL_CONF);

	while(true) {

		// Read a register:
		uint16_t raw_read = read_i2c_reg16(i2c_file_desc, REG_DATA);
        printf("Raw reading: 0x%04x\n", raw_read);
	
		// Convert byte order and shift bits into place
		uint16_t value = ((raw_read & 0xFF) << 8) | ((raw_read & 0xFF00) >> 8);
		value  = value >> 4;

		printf("Raw Read: 0x%04x  -->  Reorder & shift: 0x%04x = %8d\n", raw_read, value, value);		
		// sleep(1);

	}

	// Cleanup I2C access;
	close(i2c_file_desc);
	return 0;
}