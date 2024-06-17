#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep function
#include "./include/navigation.h"
#include "./include/list.h"
#include "./include/battery.h"
#include "./include/info.h"

#define RECHARGE_TIME 4 // Time to fully recharge the battery

// Initialize the battery with maximum capacity
void init_battery(Battery* battery) {
    battery->top = -1;
    for (int i = 0; i < MAX_BATTERY; i++) {
        battery->stack[++battery->top] = 1; // Each unit represents one unit of movement
    }
}

// Check if the battery is empty
int is_empty(Battery* battery) {
    return battery->top == -1;
}

// Check if the battery is full
int is_full(Battery* battery) {
    return battery->top == MAX_BATTERY - 1;
}

// Push (recharge) a unit of battery
void push(Battery* battery, int value) {
    if (!is_full(battery)) {
        battery->stack[++battery->top] = value;
    }
}

// Pop (consume) a unit of battery
int pop(Battery* battery) {
    if (!is_empty(battery)) {
        return battery->stack[battery->top--];
    }
    return -1; // Indicates that the battery is empty
}

// Get the current battery level
int battery_level(Battery* battery) {
    return battery->top + 1;
}

// Simulate battery recharging
void recharge_battery(Battery* battery) {
    printf("Recharging battery...\n");
    sleep(RECHARGE_TIME); // Simulate 4 seconds of charging time
    while (!is_full(battery)) {
        push(battery, 1);
    }
    printf("Battery fully recharged. Battery level: %d\n", battery_level(battery));
}

// Manage battery while moving the robot
int manage_battery_on_move(Battery* battery, int* actual, int* next_pos) {
    tMap mapa = read_map_from_file();
    if (mapa.map_data == NULL) {
        fprintf(stderr, "Failed to read map data\n");
        return -1; // Indicating error
    }

    int time = 0;
    int y_bat = next_pos[1];
    int x_bat = next_pos[0];

    if (y_bat < 0 || y_bat >= mapa.sz_y || x_bat < 0 || x_bat >= mapa.sz_x) {
        fprintf(stderr, "Next position out of map bounds\n");
        return -1; // Indicating error
    }

    int cell = mapa.map_data[8 - y_bat][x_bat];
    int level = battery_level(battery);

    if (cell == 255 || cell == 191 || cell == 127 || cell == 63) { // Navigable areas
        if (level > 1) {
            pop(battery);
            actual[0] = x_bat;
            actual[1] = y_bat;            
            time += MOVE_TIME;
            printf ("2 seconds for move\n");
            sleep(MOVE_TIME);
            printf("Moved to (%d, %d), Battery: %d\n", actual[0], actual[1], battery_level(battery));

            // Check if the battery is at 25% or less
            if (level - 1 < 2) {
                printf("Battery low. Charging at (%d, %d)\n", actual[0], actual[1]);
                recharge_battery(battery);

                FILE *file = fopen("./data/charging.txt", "a");
                if (file) {
                    fprintf(file, "Charge at (%d, %d)\n", x_bat, y_bat);
                    fclose(file);
                } else {
                    fprintf(stderr, "Failed to open charging log file\n");
                }
                time += RECHARGE_TIME;
            }
        } else {
            printf("Not enough battery to move\n");
        }
    } else if (cell == 1) { // Area with impossibility of charging
        if (level > 2) {
            pop(battery);
            actual[0] = x_bat;
            actual[1] = y_bat;
            printf ("2 seconds for move\n");
            sleep(MOVE_TIME);
            time += MOVE_TIME;
            printf("Moved to (%d, %d), Battery: %d\n", actual[0], actual[1], battery_level(battery));
            

        } else {
            printf("Battery too low to move to an area with impossibility of charge\n");
            // Recharge the battery before moving
            printf("Battery low. Charging at (%d, %d)\n", actual[0], actual[1]);
            recharge_battery(battery); 

            FILE *file = fopen("./data/charging.txt", "a");
            if (file) {
                fprintf(file, "Charge at (%d, %d)\n", actual[0], actual[1]);
                fclose(file);
            } else {
                fprintf(stderr, "Failed to open charging log file\n");
            }
            time += RECHARGE_TIME;
            pop(battery);
            actual[0] = x_bat;
            actual[1] = y_bat;            
            time += MOVE_TIME;
            printf ("2 seconds for move\n");
            sleep(MOVE_TIME);             
            printf("Moved to (%d, %d), Battery: %d\n", actual[0], actual[1], battery_level(battery));
        }
    }

    return time;
}
