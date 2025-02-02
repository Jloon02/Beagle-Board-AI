#include "hal/LED.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static bool led_initialized = false;

void led_init(void) {
    led_set_brightness_green(0);
    led_set_brightness_red(0);
    led_initialized = true;
}

void led_cleanup(void) {
    if(!led_initialized) {
        return;
    }
    led_set_brightness_green(0);
    led_set_brightness_red(0);
    led_initialized = false;
}


static void set_led_trigger(const char *trigger) {
    FILE *file = fopen(TRIGGER_FILE_NAME, "w");

    if (file == NULL) {
        perror("Error opening trigger file");
        exit(EXIT_FAILURE);
    }

    if (fprintf(file, "%s", trigger) <= 0) {
        perror("Error writing to trigger file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

static void set_led_brightness(const char *colour, int brightness) {
    FILE *file = fopen(colour, "w");

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

void led_set_brightness_green(bool brightness) {
    if(!led_initialized) return;
    set_led_brightness(GREEN_BRIGHTNESS, brightness);
}

void led_set_brightness_red(bool brightness) {
    if(!led_initialized) return;
    set_led_brightness(RED_BRIGHTNESS, brightness);
}