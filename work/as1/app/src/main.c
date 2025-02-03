// Main program to build the application

#include "reaction_game.h"

#include <stdio.h>

int main() {
    printf("Hello embedded world, from Josh!\n");
    printf("When the LEDs light up, press the joystick in that direction!\n");
    printf("Press left or right to exit\n");
    reaction_game_run();
    return 0;
}