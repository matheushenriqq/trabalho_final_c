#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "navigation.h"

#ifndef BATTERY_H
#define BATTERY_H

// Definitions for the battery
#define MAX_BATTERY 4

typedef struct {
    int stack[MAX_BATTERY];
    int top;
} Battery;

// Initialize the battery with maximum capacity
void init_battery(Battery* battery);

// Check if the battery is empty
int is_empty(Battery* battery);

// Check if the battery is full
int is_full(Battery* battery);

// Push (recharge) a unit of battery
void push(Battery* battery, int value);

// Pop (consume) a unit of battery
int pop(Battery* battery);

// Get the current battery level
int battery_level(Battery* battery);

// Battery management function when moving the robot
int manage_battery_on_move(Battery* battery, int* actual, int* next_pos);

#endif /* BATTERY_H */
