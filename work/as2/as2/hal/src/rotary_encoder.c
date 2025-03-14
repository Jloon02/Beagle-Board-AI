#include "hal/emitter.h"
#include "hal/rotary_encoder.h"
#include "hal/gpio.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

// Pin config info Rotary Encoder
#define GPIO_CHIP           GPIO_CHIP_2     // A & B
#define GPIO_LINE_NUMBER_A      7               // A
#define GPIO_LINE_NUMBER_B      8               // B

// Processor has 4 gpio chips
// on chip 2, pin #7 (functionality 7) is for A.

static bool isInitialized = false;

struct GpioLine* s_lineRotA = NULL;
struct GpioLine* s_lineRotB = NULL;

// static atomic_int counter = 0;
static bool keepRunning = false;
static pthread_t rotary_thread; // Thread handle

// Statemachine Data Structures
struct stateEvent {
    struct state* pNextState;
    void (*action)(); // function pointer
};

struct state {
    struct stateEvent a_rising;
    struct stateEvent b_rising;

    struct stateEvent a_falling;
    struct stateEvent b_falling;
};

struct state states[] = {
    // Rest
    {
        .a_rising = {&states[0], NULL},
        .b_rising = {&states[0], NULL},

        .a_falling = {&states[1], NULL},
        .b_falling = {&states[3], NULL},
    },
    // State 1
    {
        .a_rising = {&states[0], NULL},
        .b_rising = {&states[1], NULL},

        .a_falling = {&states[1], NULL},
        .b_falling = {&states[2], NULL},
    },
    // State 2
    {
        .a_rising = {&states[3], NULL},
        .b_rising = {&states[1], NULL},

        .a_falling = {&states[2], NULL},
        .b_falling = {&states[2], NULL},
    },
    // State 3
    {
        .a_rising = {&states[3], NULL},
        .b_rising = {&states[0], NULL},

        .a_falling = {&states[2], NULL},
        .b_falling = {&states[3], NULL},
    }
};

// End state machine

struct state* pCurrentState = &states[0];
struct state* pSecondState = NULL;
struct state* pSecondLastState = NULL;
struct timespec timeout = { .tv_sec = 0, .tv_nsec = 500000000 }; // 500ms timeout, prevents infinite time out for bulk wait
// We must be able to detect if the rotary encoder is turning and know if it's cc or ccc

// Thread to check the lines of A and B
static void* rotary_encoder_running(void* arg)
{
    assert(isInitialized);
    (void)arg; // Unused param

    while (keepRunning) 
    {
        // Create a bulk and add A and B lines to it
        struct gpiod_line_bulk bulkEvents;
        gpiod_line_bulk_init(&bulkEvents);
        // Extract the actual gpiod_line* from your GpioLine struct and add to the bulk
        gpiod_line_bulk_add(&bulkEvents, (struct gpiod_line*)s_lineRotA);
        gpiod_line_bulk_add(&bulkEvents, (struct gpiod_line*)s_lineRotB);

        // Wait for events on both lines in the bulk
        // int numEvents = gpiod_line_event_wait_bulk(&bulkEvents, NULL, &bulkEvents); // This causes bad stuff, don't use this
        int numEvents = gpiod_line_event_wait_bulk(&bulkEvents, &timeout, &bulkEvents);

        // Wait for GPIO from A and B
        for (int i = 0; i < numEvents; i++) 
        {
            // Get line handle for this event
            struct gpiod_line *line_handle = gpiod_line_bulk_get_line(&bulkEvents, i);

            unsigned int this_line_number = gpiod_line_offset(line_handle);

            struct gpiod_line_event event;
            if (gpiod_line_event_read(line_handle, &event) == -1) {
                perror("Line Event");
                exit(EXIT_FAILURE);
            }
            
            // Run state machine
            bool isA = (this_line_number == GPIO_LINE_NUMBER_A);
            bool isB = (this_line_number == GPIO_LINE_NUMBER_B);
            assert(isA || isB);     // Make sure they are either from A or B

            // Determine if it's a rising or falling edge
            bool isRising = (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE);

            /*
                How to keep track if we managed to do a cc or ccw cycle?
                Idea:
                [REST, 2nd state ... 2nd to last state, REST]
                we know that the starting and ending will be rest. We check the 2nd state and the 2nd to last state
                if they match, then we know not a full cycle happened.
                If they don't match, if the 2nd state is 1 and the 2nd to last state is 3, we know we got a clockwise cycle
                if the 2nd state is 3 and the 2nd to last state is 1 then we know it's a counter clockwise cycle.
            */
            
            struct state* nextState = NULL;
            if (isA) {
                nextState = isRising ? pCurrentState->a_rising.pNextState 
                                     : pCurrentState->a_falling.pNextState;
            } else if (isB) {
                nextState = isRising ? pCurrentState->b_rising.pNextState 
                                     : pCurrentState->b_falling.pNextState;
            }

            // As long as there is a next state
            if (nextState != NULL) {
                pSecondLastState = pCurrentState;
                pCurrentState = nextState;

                // Store the first state that appears after initial REST (the second state)
                if (pSecondState == NULL && pCurrentState != &states[0]) {
                    pSecondState = pCurrentState;
                }

                // If the current state is REST, that means we now need to determine if we make a cc or ccw or none cycle
                if (pCurrentState == &states[0]) {
                    // Make sure neither are NULL
                    if (pSecondState && pSecondLastState) {
                        // Clockwise check
                        if (pSecondState == &states[1] && pSecondLastState == &states[3]) {
                            Emitter_increment_flashing();
                        }
                        // Counter clockwise check
                        else if (pSecondState == &states[3] && pSecondLastState == &states[1]) {
                            Emitter_decrement_flashing();
                        }
                        // Failed cycle
                        else {
                            printf("Failed rotation detected \n");
                        }
                    }
                    pSecondState = NULL;
                    pSecondLastState = NULL;
                }
                
            }
        }

    }
    return NULL;
}

void Rotary_encoder_init(void)
{
    assert(!isInitialized);
    s_lineRotA = Gpio_openForEvents(GPIO_CHIP, GPIO_LINE_NUMBER_A);
    s_lineRotB = Gpio_openForEvents(GPIO_CHIP, GPIO_LINE_NUMBER_B);
    gpiod_line_request_both_edges_events((struct gpiod_line*)s_lineRotA, "Rotary A");
    gpiod_line_request_both_edges_events((struct gpiod_line*)s_lineRotB, "Rotary B");

    isInitialized = true;
    keepRunning = true;

    // Create the rotary encoder thread inside the module
    if (pthread_create(&rotary_thread, NULL, rotary_encoder_running, NULL) != 0) {
        perror("Failed to create rotary encoder thread");
        exit(EXIT_FAILURE);
    }

}

void Rotary_encoder_cleanup(void)
{
    assert(isInitialized);
    keepRunning = false;
    // Wake up the thread if needed
    struct timespec delay = { .tv_sec = 0, .tv_nsec = 100000000 }; // 100ms delay
    nanosleep(&delay, NULL);
    pthread_cancel(rotary_thread);
    pthread_join(rotary_thread, NULL);  // Ensure thread has finished before proceeding
    Gpio_close(s_lineRotA);
    Gpio_close(s_lineRotB);
    isInitialized = false;
}