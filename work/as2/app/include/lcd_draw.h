// lcd_draw.h

#ifndef _LCD_DRAW_H_
#define _LCD_DRAW_H_

/*
    lcd_draw is responsible for printing out the screen and keeping
    track of the 3 values we need to print which are: frequency (flash), dips, and max ms
*/

void Lcd_draw_init();
void Lcd_draw_cleanup();

// // Functions to update the important values
// void lcd_draw_updateFrequency(int frequency);
// void lcd_draw_updateDips(int dips);
// void lcd_draw_updateMs(float ms);

// update screen auto is to refresh the screen when values change
// void lcd_draw_updateScreenAuto(void);
// void lcd_draw_updateScreen(char* message);

#endif