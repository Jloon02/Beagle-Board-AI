#include "hal/emitter.h"

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static bool is_initialized = false;

static pthread_mutex_t freqMutex = PTHREAD_MUTEX_INITIALIZER;

static const char *emitter_path = "/dev/hat/pwm/GPIO12";
static const char *enable = "/enable";
static const char *duty_cycle = "/duty_cycle";
static const char *period = "/period";

static bool is_1hz_flashing = false;  // Flag to control 1 Hz flashing
static pthread_t flashing_thread;     // Thread for 1 Hz flashing
static int current_frequency = 10;

static UpdateLCDCallback update_lcd_callback = NULL;

static pthread_mutex_t frequency_mutex;

static void default_flashing(void) 
{
    // Flash at 10 Hz
    write_file(duty_cycle, "0");
    write_file(period, DEFAULT_PERIOD);
    write_file(duty_cycle, DUTY_CYCLE);
    write_file(enable, "1");
    printf("Current frequency: %d Hz\n", current_frequency);
}

void set_update_lcd_callback(UpdateLCDCallback callback)
{
    update_lcd_callback = callback;
}

int Emitter_get_frequency(void)
{
    int freq;
    pthread_mutex_lock(&freqMutex);
    freq = current_frequency;
    pthread_mutex_unlock(&freqMutex);
    return freq;
}

void Emitter_init(void)
{
    printf("Emitter - Initalizing\n");
    assert(!is_initialized);
    default_flashing();
    is_initialized = true;
    pthread_mutex_init(&frequency_mutex, NULL);
}

void Emitter_cleanup(void)
{
    assert(is_initialized);
    write_file(enable, 0);
    is_initialized = false;
    pthread_mutex_destroy(&frequency_mutex);
}

void stop_1hz_flashing(void)
{
    if (is_1hz_flashing) {
        printf("Stopping 1Hz\n");
        is_1hz_flashing = false;
        pthread_join(flashing_thread, NULL);
    }
}

void Emitter_increment_flashing(void)
{
    pthread_mutex_lock(&frequency_mutex);
    if (current_frequency < MAX_HZ) {
        current_frequency++;
    }

    if (update_lcd_callback) {
        update_lcd_callback(current_frequency);
    }

    write_file(enable, "1");

    // Calculate the new period in nanoseconds (1 / frequency in Hz)
    long new_period = (1000000000L / current_frequency);  // Period in nanoseconds
    printf("Current frequency: %d Hz\n", current_frequency);
    pthread_mutex_unlock(&frequency_mutex);

    // Update the period (time for one complete cycle)
    char period_str[20];
    snprintf(period_str, sizeof(period_str), "%ld", new_period);
    write_file(period, period_str);

    // Set duty cycle to 50% for a clean flash
    long duty_cycle_ns = new_period / 2;  // Half of the period for 50% duty cycle

    char duty_cycle_str[20];
    snprintf(duty_cycle_str, sizeof(duty_cycle_str), "%ld", duty_cycle_ns);
    write_file(duty_cycle, duty_cycle_str);
}

void Emitter_decrement_flashing(void)
{
    pthread_mutex_lock(&frequency_mutex);
    // Decrement frequency, allow it to reach 0 Hz
    if (current_frequency > 0) {
        current_frequency--;  // Decrease frequency by 1 Hz
    }

    if (update_lcd_callback) {
        update_lcd_callback(current_frequency);
    }

    // If frequency is 0, stop the flashing (set period to 0, disable output)
    if (current_frequency == 0) {
        // stop_1hz_flashing();
        write_file(period, "0");  // Set period to 0 (no flashing)
        write_file(enable, "0");  // Disable the emitter output
    } 
    else {
        // stop_1hz_flashing();
        // Calculate the new period in nanoseconds (1 / frequency in Hz)
        long new_period = (1000000000L / current_frequency);  // Period in nanoseconds
        
        // Update the period (time for one complete cycle)
        char period_str[20];
        snprintf(period_str, sizeof(period_str), "%ld", new_period);
        write_file(period, period_str);

        // Set duty cycle to 50% for a clean flash
        long duty_cycle_ns = new_period / 2;  // Half of the period for 50% duty cycle
        char duty_cycle_str[20];
        snprintf(duty_cycle_str, sizeof(duty_cycle_str), "%ld", duty_cycle_ns);
        write_file(duty_cycle, duty_cycle_str);
    }
    pthread_mutex_unlock(&frequency_mutex);

    printf("Current frequency: %d Hz\n", current_frequency);
}

void write_file(const char* file, const char* value)
{
    char path[256];  // Buffer to store the full path
    snprintf(path, sizeof(path), "%s/%s", emitter_path, file);

    // Open file for writing
    FILE *openFile = fopen(path, "w");
    if (openFile == NULL) {
        perror("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    
    int charWritten = fprintf(openFile, "%s", value);
    if (charWritten <= 0) {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }

    fclose(openFile);

}