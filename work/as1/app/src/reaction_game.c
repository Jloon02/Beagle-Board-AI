// Main reaction game loop

#include "reaction_game.h"
#include "time_functions.h"

#include "hal/joystick.h"
#include "hal/LED.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

static long long get_random_constraint() {
    return (rand() % 2500) + 500; // Between 500 and 3000
}

static void flash_green() {
	for(int i=0; i<5; i++) {
		led_set_brightness_green(1);
		sleepForMs(100);
		led_set_brightness_green(0);
		sleepForMs(100);
	}
}

static void flash_red() {
		for(int i=0; i<5; i++) {
		led_set_brightness_red(1);
		sleepForMs(100);
		led_set_brightness_red(0);
		sleepForMs(100);
	}
}

// For testing Purposes
void test() {
	printf("testing..\n");
	joystick_init();
	while(1) {
		JoystickDirection dir = joystick_get_direction();

        switch (dir) {
            case JOYSTICK_UP:
                printf("Joystick Direction: UP\n");
                break;
            case JOYSTICK_DOWN:
                printf("Joystick Direction: DOWN\n");
                break;
            case JOYSTICK_LEFT:
                printf("Joystick Direction: LEFT\n");
                break;
            case JOYSTICK_RIGHT:
                printf("Joystick Direction: RIGHT\n");
                break;
            default:
                printf("Joystick Direction: NONE\n");
                break;
        }
		sleepForMs(10);
	}
}

// Main Game Loop
void reaction_game_run()
{
	srand(time(NULL));

	led_init();
	joystick_init();

	long long best_time = -1;

	while(1) {
		printf("Get ready... \n");

		// flash green and red 4 times
		for(int i=0; i<4; i++) {
			led_set_brightness_green(1);
			sleepForMs(250);
			led_set_brightness_green(0);
			led_set_brightness_red(1);
			sleepForMs(250);
			led_set_brightness_red(0);
		}

		// Check if joystick is neutral
		while(joystick_get_direction() != JOYSTICK_NONE) {
			printf("Please let go of the joystick!\n");
			sleepForMs(100);
		}

		// Wait for a random amount of given time
		sleepForMs(get_random_constraint());

		// If user presses too soon, restart
		if(joystick_get_direction() != JOYSTICK_NONE) {
			printf("Too soon!!\n");
			if(joystick_get_direction() == JOYSTICK_LEFT || joystick_get_direction() == JOYSTICK_RIGHT) {
				printf("User selected to quit.\n");
				break;
			}
			continue;
		}

		// Random direction (Up/down) for reaction test
		JoystickDirection test_direction;
		int random_dir = rand() % 2;
		if(random_dir) {
			test_direction = JOYSTICK_UP;
		} else {
			test_direction = JOYSTICK_DOWN;
		}
		
		// Prompt user
		if(test_direction == JOYSTICK_UP) {
			printf("Press UP now!\n");
			led_set_brightness_green(1);
		} else {
			printf("Press DOWN now!\n");
			led_set_brightness_red(1);
		}

		// Starting timer
		long long start_time = getTimeInMs();

		// Get user input
		JoystickDirection user_direction;
		while(1) {
			user_direction = joystick_get_direction();
			if(user_direction != JOYSTICK_NONE) break;

			// Timeout
			if(getTimeInMs() - start_time > 5000) {
				printf("No input within 5000ms; quitting!\n");
				led_cleanup();
				joystick_cleanup();
				exit(EXIT_SUCCESS);
			}
		}

		// End timer
		long long result_time = getTimeInMs() - start_time;

		// Turn off LED
		led_set_brightness_red(0);
		led_set_brightness_green(0);

		// Calculate user result
		if(user_direction == test_direction) {
			printf("Correct!\n");

			if(best_time == -1 || result_time < best_time) {
				best_time = result_time;
				printf("New best time!\n");
			}

			printf("Your reaction time was %lldms; best so far in game is %lldms.\n", result_time, best_time);
			flash_green();
		} else if(user_direction == JOYSTICK_LEFT || user_direction == JOYSTICK_RIGHT) {
			printf("User selected to quit.\n");
			break;
		} else {
			printf("Incorrect.\n");
			flash_red();
		}
	}

	led_cleanup();
	joystick_cleanup();
}