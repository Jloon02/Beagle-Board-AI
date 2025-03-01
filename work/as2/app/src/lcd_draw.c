#include "lcd_draw.h"
#include "periodTimer.h"
#include "sampler.h"
#include "hal/emitter.h"
#include "DEV_Config.h"
#include "LCD_1in54.h"
#include "GUI_Paint.h"
#include "GUI_BMP.h"

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()
#include <signal.h>     //signal()
#include <stdbool.h>
#include <unistd.h>     //usleep()

#define THRESHOLD 0.0001

static UWORD *s_fb;
static pthread_t lcdUpdateThread;
static bool updateLCD = true;
static bool isInitialized = false;

static int current_frequency = 10;
static int current_dips = 0;
static float current_max_ms = 0.0;

static const int line_buffer = 20;
static const int buffer_size = 24;

static void* lcdUpdateFunction(void* arg)
{
    (void) arg;
    // Track last frequency
    int last_frequency = -1;
    int last_dips = -1;
    float last_ms = -1.0;

    while (updateLCD) {
        Period_statistics_t stats;
        Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &stats);

        int current_freq = Emitter_get_frequency();  // Get latest frequency
        if (current_freq != last_frequency) {  // Only update LCD if frequency changed
            lcd_draw_updateFrequency(current_freq);
            last_frequency = current_freq;  // Save new frequency
        }

        // More for Dips
        int current_dips_check = Sample_getDipsCount();
        if (current_dips_check != last_dips) {
            lcd_draw_updateDips(current_dips_check);
            last_dips = current_dips_check;
        }

        // More for ms
        float current_max_ms_check = stats.maxPeriodInMs;
        // int current_max_ms_check = 0;
        if (fabs(current_max_ms_check - last_ms) > THRESHOLD) {
            lcd_draw_updateMs(current_max_ms_check);
            last_ms = current_max_ms_check;
        }

        usleep(50000);  // Sleep to avoid excessive polling (50ms = 20 updates/sec)
    }
    return NULL;
}

void Lcd_draw_init()
{
    assert(!isInitialized);
    
    // Module Init
	if(DEV_ModuleInit() != 0){
        DEV_ModuleExit();
        exit(0);
    }
	
    // LCD Init
    DEV_Delay_ms(2000);
	LCD_1IN54_Init(HORIZONTAL);
	LCD_1IN54_Clear(WHITE);
	LCD_SetBacklight(1023);

    UDOUBLE Imagesize = LCD_1IN54_HEIGHT*LCD_1IN54_WIDTH*2;
    if((s_fb = (UWORD *)malloc(Imagesize)) == NULL) {
        perror("Failed to apply for black memory");
        exit(0);
    }
    isInitialized = true;

    pthread_create(&lcdUpdateThread, NULL, lcdUpdateFunction, NULL);

}

void Lcd_draw_cleanup()
{
    assert(isInitialized);
    updateLCD = false;
    pthread_join(lcdUpdateThread, NULL);
    // Module Exit
    free(s_fb);
    s_fb = NULL;
	DEV_ModuleExit();
    isInitialized = false;
}

void lcd_draw_updateFrequency(int frequency)
{
    current_frequency = frequency;
    lcd_draw_updateScreenAuto();
}

void lcd_draw_updateDips(int dips)
{
    current_dips = dips;
    lcd_draw_updateScreenAuto();
}

void lcd_draw_updateMs(float ms)
{
    current_max_ms = ms;
    lcd_draw_updateScreenAuto();
}

void lcd_draw_updateScreenAuto(void)
{
    assert(isInitialized);

    const int x = 5;
    const int y = 70;
    int nextY = y;


    // Initialize the RAM frame buffer to be blank (white)
    Paint_NewImage(s_fb, LCD_1IN54_WIDTH, LCD_1IN54_HEIGHT, 0, WHITE, 16);
    Paint_Clear(WHITE);

    // Draw into the RAM frame buffer
    // WARNING: Don't print strings with `\n`; will crash!
    char introBuffer[buffer_size];
    snprintf(introBuffer, sizeof(introBuffer), "Josh & Nelson's!");
    Paint_DrawString_EN(x, nextY, introBuffer, &Font16, WHITE, BLACK);

    nextY += line_buffer;

    char freqBuffer[buffer_size];
    snprintf(freqBuffer, sizeof(freqBuffer), "Flash @ %d Hz", current_frequency);
    Paint_DrawString_EN(x, nextY, freqBuffer, &Font16, WHITE, BLACK);

    nextY += line_buffer;

    char dipsBuffer[buffer_size];
    snprintf(dipsBuffer, sizeof(dipsBuffer), "Dips = %d", current_dips);
    Paint_DrawString_EN(x, nextY, dipsBuffer, &Font16, WHITE, BLACK);

    nextY += line_buffer;

    char msBuffer[buffer_size];
    snprintf(msBuffer, sizeof(msBuffer), "Max ms: %.2f", current_max_ms);
    Paint_DrawString_EN(x, nextY, msBuffer, &Font16, WHITE, BLACK);

    nextY += line_buffer;

    LCD_1IN54_DisplayWindows(x, y, LCD_1IN54_WIDTH, nextY+20, s_fb);
    
}

void lcd_draw_updateScreen(char* message)
{
    assert(isInitialized);

    const int x = 5;
    const int y = 70;

    // Initialize the RAM frame buffer to be blank (white)
    Paint_NewImage(s_fb, LCD_1IN54_WIDTH, LCD_1IN54_HEIGHT, 0, WHITE, 16);
    Paint_Clear(WHITE);

    // Draw into the RAM frame buffer
    // WARNING: Don't print strings with `\n`; will crash!
    Paint_DrawString_EN(x, y, message, &Font16, WHITE, BLACK);

    // Send the RAM frame buffer to the LCD (actually display it)
    // Option 1) Full screen refresh (~1 update / second)
    // LCD_1IN54_Display(s_fb);
    // Option 2) Update just a small window (~15 updates / second)
    //           Assume font height <= 20
    LCD_1IN54_DisplayWindows(x, y, LCD_1IN54_WIDTH, y+20, s_fb);
}