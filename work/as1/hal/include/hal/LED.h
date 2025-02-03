#ifndef _LED_H
#define _LED_H

#include <stdbool.h>

// LED control file paths
#define GREEN_INDEX 0
#define RED_INDEX 1

#define NUM_LEDS 2 // Red and Green

typedef struct {
    const char *colour;
    const char *trigger_path;
    const char *brightness_path;
} LED;

void led_init(void);
void led_cleanup(void);
void led_set_brightness_green(bool brightness);
void led_set_brightness_red(bool brightness);

#endif