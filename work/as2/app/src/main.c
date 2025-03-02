// Main program to build the application
// Has main(); does initialization and cleanup and perhaps some basic logic.

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "hal/emitter.h"
#include "hal/gpio.h"
#include "hal/rotary_encoder.h"
#include "lgpio.h"
#include "lcd_draw.h"
#include "sampler.h"
#include "udp_server.h"
#include "timeFunction.h"
#include "terminal_output.h"
#include "periodTimer.h"
#include "hal/light_detector.h"

int main()
{
    printf("Starting Program.\n");

    // Initialize all modules; HAL modules first
    Gpio_initialize();
    Emitter_init();
    Period_init();
    Lcd_draw_init();
    Rotary_encoder_init();
    Sampler_init();
    LightDetector_init();
    UdpServer_start();
    TerminalOutput_init();
    

    while(UdpServer_isOnline()) {
        Sampler_moveCurrentDataToHistory();
        sleep_for_ms(1000);
    }

    printf("Cleaning up modules.\n");
    TerminalOutput_cleanup();
    UdpServer_stop();
    LightDetector_cleanup();
    Sampler_cleanup();
    Rotary_encoder_cleanup();
    Lcd_draw_cleanup();
    Period_cleanup();
    Emitter_cleanup();
    Gpio_cleanup();
    
    printf("Program completely successfully.\n");
    return 0;
}