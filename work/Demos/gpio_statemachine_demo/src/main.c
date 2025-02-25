/*
	State machine for GPIO
*/

// Relies on the gpiod library.
// Insallation for cross compiling:
//      (host)$ sudo dpkg --add-architecture arm64
//      (host)$ sudo apt update
//      (host)$ sudo apt install libgpdiod-dev:arm64
// GPIO: https://www.ics.com/blog/gpio-programming-exploring-libgpiod-library
// Example: https://github.com/starnight/libgpiod-example/blob/master/libgpiod-input/main.c

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "gpio.h"
#include "btn_statemachine.h"

#include <time.h>

int main(void) 
{
    // Startup & Initialization
    Gpio_initialize();
    BtnStateMachine_init();

    // TESTING State machine
    while (true) {
        // TODO: This should be on it's own thread!
        BtnStateMachine_doState();

        printf("Counter at %+5d\n", BtnStateMachine_getValue());
    }

    BtnStateMachine_cleanup();
    Gpio_cleanup();

    printf("\nDone!\n");
    return 0;
}
