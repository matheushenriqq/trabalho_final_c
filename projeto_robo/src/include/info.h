#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "navigation.h"
#include "list.h"

#ifndef INFO_H
#define INFO_H

// Structure to store positions
typedef struct {
    int x;
    int y;
} Position;

// Function to print the map
void print_map(void);

// Function to check for gold at the current position
int gold(int actual[2]);

// Function to check for silver at the current position
int silver(int actual[2]);

// Function to check if charging is impossible at the current position
void charge_impossible(int actual[2]);

// Function to check for bronze at the current position
int bronze(int actual[2]);

// Function to clear the content of a file
void clear_file(const char *filepath);

// Function to list the way taken by the robot
void list_way(tList *list, int actual[2]);

// Function to identify obstacles and write their positions to a file
int obstacles(int actual[2], Position *registered_positions, int *registered_count);

// Function to check if a position already exists in the list of registered positions
int position_exists(Position *positions, int count, int x, int y);

#endif /* INFO_H */
