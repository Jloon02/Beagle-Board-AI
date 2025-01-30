#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRIGGER_FILE_NAME "/sys/class/leds/ACT/trigger"
#define BRIGHTNESS_FILE_NAME "/sys/class/leds/ACT/brightness"

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

int main() {
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

    return 0;
}
