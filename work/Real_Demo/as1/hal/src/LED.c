// HAL module for controlling the LED on BeagleBoard

#include "hal/LED.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static bool led_initialized = false;

static LED leds[NUM_LEDS] = {
    {"Green", "/sys/class/leds/ACT/trigger", "/sys/class/leds/ACT/brightness"},
    {"Red", "/sys/class/leds/PWR/trigger", "/sys/class/leds/PWR/brightness"}
};

// Write to brightness LED file
static void write_led_brightness(int index, int brightness) {
    FILE *file = fopen(leds[index].brightness_path, "w");

    if (file == NULL) {
        perror("Error opening brightness file");
        exit(EXIT_FAILURE);
    }

    if (fprintf(file, "%d", brightness) <= 0) {
        perror("Error writing to brightness file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

void led_init(void) {
    // Turn off all lights
    for(int i=0; i<NUM_LEDS; i++) {
        write_led_brightness(i, 0);
    }

    led_initialized = true;
}

void led_cleanup(void) {
    if(!led_initialized) {
        return;
    }
    
    for(int i=0; i<NUM_LEDS; i++) {
        write_led_brightness(i, 0);
    }

    led_initialized = false;
}

void led_set_brightness_green(bool brightness) {
    if(!led_initialized) return;
    write_led_brightness(GREEN_INDEX, brightness);
}

void led_set_brightness_red(bool brightness) {
    if(!led_initialized) return;
    write_led_brightness(RED_INDEX, brightness);
}