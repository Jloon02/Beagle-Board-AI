// emitter.h
// This module focuses on maintaining the emitter
// by changing its frequency (by altering the period)

#ifndef _EMITTER_H_
#define _EMITTER_H_


#define DEFAULT_PERIOD "100000000"
#define DUTY_CYCLE "50000000"
#define MAX_HZ 500
#define MIN_HZ 0
#define MAX_PERIOD_NS 469754879 

#include <stdbool.h>

// Define a callback type for updating the LCD
// A callback is a function that is passed as an argument to another function, 
// and it's executed at a later time, often when some event or condition occurs. 
typedef void (*UpdateLCDCallback)(int frequency);
void set_update_lcd_callback(UpdateLCDCallback callback);

void Emitter_init(void);
void Emitter_cleanup(void);
int Emitter_get_frequency(void);
void Emitter_increment_flashing(void);
void Emitter_decrement_flashing(void);

void write_file(const char* file, const char* value);
#endif