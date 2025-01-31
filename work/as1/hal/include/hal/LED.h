#ifndef _LED_H
#define _LED_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// LED control file paths
#define TRIGGER_FILE_NAME "/sys/class/leds/ACT/trigger"
#define BRIGHTNESS_FILE_NAME "/sys/class/leds/ACT/brightness"

// Function prototypes
void set_led_trigger(const char *led_name, const char *trigger);
void set_led_brightness(const char *led_name, int brightness);
void sleep_for(double seconds);

#endif