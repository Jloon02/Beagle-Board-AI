#ifndef _LED_H
#define _LED_H

#include <stdbool.h>

// LED control file paths
#define TRIGGER_FILE_NAME "/sys/class/leds/ACT/trigger"
#define GREEN_BRIGHTNESS "/sys/class/leds/ACT/brightness"
#define RED_BRIGHTNESS "/sys/class/leds/PWR/brightness"

// Function prototypes
// static void set_led_trigger(const char *trigger);
// static void set_led_brightness(const char *colour, int brightness);
void led_init(void);
void led_cleanup(void);
void led_set_brightness_green(bool brightness);
void led_set_brightness_red(bool brightness);

#endif