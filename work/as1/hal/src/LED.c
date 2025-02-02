#include "hal/LED.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// LED control file paths
#define TRIGGER_FILE_NAME "/sys/class/leds/ACT/trigger"
#define BRIGHTNESS_FILE_NAME "/sys/class/leds/ACT/brightness"

static bool led_initialized = false;

void led_init(void) {
    led_initialized = true;
}


void set_led_trigger(const char *led_name, const char *trigger) {
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

void set_led_brightness(const char *led_name, int brightness) {
    FILE *file = fopen(BRIGHTNESS_FILE_NAME, "w");

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

void sleep_for(double seconds) {
    struct timespec req;
    req.tv_sec = (time_t)seconds;
    req.tv_nsec = (seconds - req.tv_sec) * 1e9;
    nanosleep(&req, NULL);
}
