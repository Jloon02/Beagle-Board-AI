// terminal_output.c

#include "terminal_output.h"
#include "timeFunction.h"
#include "sampler.h"
#include "periodTimer.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

// Constants
#define PRINT_MS_INTERVAL 1000  // Print every 1 second
#define SAMPLES_PER_LINE 10  // Print 10 evenly spaced samples

// Global variables
static pthread_t printThread;
static bool isPrinting = false;
static bool isPrintInitialized = false;

// Thread function to print terminal output
static void* printThreadFunc(void* none) {
    (void)none; // Suppress unused parameter warning
    
    while (isPrinting) {
        // Get timing jitter information from periodTimer module
        Period_statistics_t stats;
        Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &stats);

        // Get information to display
        int historySize = Sampler_getHistorySize();
        int pwmFrequency = 10;  // TODO: replace with pwm function @Nelson
        double averageLightLevel = Sampler_getAverageReading();
        int dips = Sample_getDipsCount();

        // First line of output
        printf("#Smpl/s = %d Flash @ %dHz avg = %.3fV dips = %d Smpl ms[%6.3f, %6.3f] avg %6.3f/%d\n",
               historySize, pwmFrequency, averageLightLevel, dips,
               stats.minPeriodInMs, stats.maxPeriodInMs, stats.avgPeriodInMs, stats.numSamples);

        // Second line of output
        // 10 evenly spaced samples
        int size;
        double *history = Sampler_getHistory(&size);
        if (history != NULL && size > 0) {
            for (int i = 0; i < SAMPLES_PER_LINE; i++) {
                int index = (i * size) / SAMPLES_PER_LINE;
                if (index < size) {
                    printf("%d:%.3f ", index, history[index]);
                }
            }
            printf("\n");
        } else {
            printf("No history data available.\n");
        }
        free(history);

        // Sleep for 1 second
        sleep_for_ms(PRINT_MS_INTERVAL);
    }
    return NULL;
}

// Initialize the terminal output module
void TerminalOutput_init(void) {
    assert(!isPrintInitialized);

    isPrintInitialized = true;
    isPrinting = true;

    // Start the print thread
    if (pthread_create(&printThread, NULL, printThreadFunc, NULL) != 0) {
        perror("Failed to create print thread");
        exit(EXIT_FAILURE);
    }
}

// Clean up resources used by the terminal output module
void TerminalOutput_cleanup(void) {
    assert(isPrintInitialized);

    isPrintInitialized = false;
    isPrinting = false;
    pthread_join(printThread, NULL);
}