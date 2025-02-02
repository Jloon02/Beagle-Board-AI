#ifndef _LED_H
#define _LED_H

// Function prototypes
void set_led_trigger(const char *led_name, const char *trigger);
void set_led_brightness(const char *led_name, int brightness);
void sleep_for(double seconds);

#endif