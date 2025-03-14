// sampler.c

#include "sampler.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> //memcpy
#include <stdbool.h>
#include "hal/light_detector.h"
#include "periodTimer.h"
#include "timeFunction.h"

// Define constants
#define SAMPLE_INTERVAL_MS 1 
#define MAX_HISTORY_SIZE 1000 // Maximum of 1 second of data stored (1000ms = 1s)
#define DIP_DROP 0.1 // Light level must drop by at least 0.1V to count as a dip
#define DIP_HYSTERESIS 0.03 // Prevent noise when analyzing dips

#define SMOOTHING 0.999

// Static variables
static pthread_t samplingThread;
static bool isThreadSampling = false;
static double currentBuffer[MAX_HISTORY_SIZE];
static double historyBuffer[MAX_HISTORY_SIZE];
static int currentIndex = 0;
static long long totalSamplesTaken = 0;
static double exponentialAverage = 0.0; // exponential averaging
static int historySize = 0; // Number of samples in history buffer
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// Thread to sample the light levels
static void* samplingThreadFunc(void* none) {
    (void)none; // Suppress unused parameter warning
    while (isThreadSampling) {
        // Reading the light level from the ADC
        double lightLevel = LightDetector_readVoltage();

        // Update the exponential moving average
        if (totalSamplesTaken == 0) { // First time sampling
            exponentialAverage = lightLevel;
        } else {
            // exponential smoothing over all light samples weight of 99.9%
            exponentialAverage = SMOOTHING * exponentialAverage + (1-SMOOTHING) * lightLevel;
        }

        // Store the sample in the current buffer
        // Lock so nowhere else can access
        pthread_mutex_lock(&mutex);
        if (currentIndex < MAX_HISTORY_SIZE) {
            currentBuffer[currentIndex] = lightLevel;
            currentIndex++;
        }
        
        totalSamplesTaken++;
        pthread_mutex_unlock(&mutex);

        // Mark the time event
        Period_markEvent(PERIOD_EVENT_SAMPLE_LIGHT);

        // Sleep for 1ms
        sleep_for_ms(1);
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
    // Move the current sampling to history buffer
    pthread_mutex_lock(&mutex);
    
    memcpy(historyBuffer, currentBuffer, currentIndex * sizeof(double));
    historySize = currentIndex;
    currentIndex = 0; // resetting the current buffer index
    pthread_mutex_unlock(&mutex);
}

// Get the number of samples collected during the previous complete second.
int Sampler_getHistorySize(void) {
    return historySize;
}

// Get a copy of the samples in the sample history.
// Returns a newly allocated array and sets `size` to be the
// number of elements in the returned array (output-only parameter).
// The calling code must call free() on the returned pointer.
// Note: It provides both data and size to ensure consistency.
double* Sampler_getHistory(int *size) {
    //int data_size = *size;
    double* historyCopy = (double*)malloc(historySize * sizeof(double));

    if (historyCopy == NULL) {
        perror("Failed to allocate memory for history copy");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&mutex);

    memcpy(historyCopy, historyBuffer, historySize * sizeof(double));
    *size = historySize;

    pthread_mutex_unlock(&mutex);
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

// Count and return the number of dips of V in the history
int Sample_getDipsCount(void) {
    int dipCount = 0;
    bool isDipped = false;

    pthread_mutex_lock(&mutex);
    double average = exponentialAverage;

    for (int i = 0; i < MAX_HISTORY_SIZE; i++) {
        double sample = historyBuffer[i];

        // Checking if the sample is below DIP_DROP threshold
        if (!isDipped && sample < (average - DIP_DROP)) {
            dipCount++;  // New dip
            isDipped = true;
        }

        // Checking if sample has risen using hysteresis
        if (isDipped && sample > (average - DIP_DROP + DIP_HYSTERESIS)) {
            isDipped = false;  // Dip end
        }
    }
    pthread_mutex_unlock(&mutex);

    return dipCount;
}