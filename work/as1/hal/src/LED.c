#include "hal/LED.h"

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
