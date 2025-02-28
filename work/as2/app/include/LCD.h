// LCD.h
// This module control the LCD screen of the beagle board

#ifndef _LCD_H_
#define _LCD_H_

void LCD_init();
void LCD_cleanup();

void LCD_updateScreen(char* message);

#endif