// sampler.c

#include "sampler.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "hal/light_detector.h"
#include "timeFunction.h"

// Define constants
#define SAMPLE_INTERVAL_MS 1 
#define HISTORY_SIZE 1000 // 1 second of data stored (1000ms = 1s)

// Static variables
static pthread_t samplingThread;
static bool isThreadSampling = false;
static double currentBuffer[HISTORY_SIZE];
static double historyBuffer[HISTORY_SIZE];
static int currentIndex = 0;
static long long totalSamplesTaken = 0;
static double exponentialAverage = 0.0;
//static int collectedHistory = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// Thread to sample the light levels
static void* samplingThreadFunc(void* none) {
    while (isThreadSampling) {
        // Reading the light level from the ADC
        double lightLevel = LightDetector_readVoltage();

        // Update the exponential moving average
        if (totalSamplesTaken == 0) { // First time sampling
            exponentialAverage = lightLevel;
        } else {
            // exponential smoothing over all light samples weight of 99.9%
            exponentialAverage = 0.999 * exponentialAverage + 0.001 * lightLevel;
        }

        // Store the sample in the current buffer
        // Lock so nowhere else can access
        pthread_mutex_lock(&mutex);
        currentBuffer[currentIndex] = lightLevel;
        currentIndex = (currentIndex + 1) % HISTORY_SIZE;
        totalSamplesTaken++;
        pthread_mutex_unlock(&mutex);

        // Sleep for 1ms
        sleep_for_ms(SAMPLE_INTERVAL_MS * HISTORY_SIZE);
    }
    return NULL;
}

// Begin/end the background thread which samples light levels.
void Sampler_init(void) {
    isThreadSampling = true;
    currentIndex = 0;
    totalSamplesTaken = 0;
    exponentialAverage = 0.0;

    // Start the sampling thread
    if (pthread_create(&samplingThread, NULL, samplingThreadFunc, NULL) != 0) {
        perror("Error on creating sampling thread");
        exit(EXIT_FAILURE);
    }
}

// Clean up thread
void Sampler_cleanup(void) {
    isThreadSampling = false;
    pthread_join(samplingThread, NULL);
    pthread_mutex_destroy(&mutex);
}

// Must be called once every 1s.
// Moves the samples that it has been collecting this second into
// the history, which makes the samples available for reads (below).
void Sampler_moveCurrentDataToHistory(void) {
    //collectedHistory = 0;
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < HISTORY_SIZE; i++) {
        historyBuffer[i] = currentBuffer[i];
    }
    pthread_mutex_unlock(&mutex);
}

// Get the number of samples collected during the previous complete second.
int Sampler_getHistorySize(void) {
    return HISTORY_SIZE;
}

// Get a copy of the samples in the sample history.
// Returns a newly allocated array and sets `size` to be the
// number of elements in the returned array (output-only parameter).
// The calling code must call free() on the returned pointer.
// Note: It provides both data and size to ensure consistency.
double* Sampler_getHistory(int *size) {
    double* historyCopy = malloc(HISTORY_SIZE * sizeof(double));
    if (historyCopy == NULL) {
        perror("Failed to allocate memory for history copy");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < HISTORY_SIZE; i++) {
        historyCopy[i] = historyBuffer[i];
    }
    pthread_mutex_unlock(&mutex);

    *size = HISTORY_SIZE;
    return historyCopy;
}

// Get the average light level (not tied to the history).
double Sampler_getAverageReading(void) {
    return exponentialAverage;
}

// Get the total number of light level samples taken so far.
long long Sampler_getNumSamplesTaken(void) {
    return totalSamplesTaken;
}