#ifndef NAVIGATION_H
#define NAVIGATION_H

#define MAX_COORDS 1000 // Maximum number of coordinates
#define MAX_ROWS 100 // Maximum number of map rows
#define MAX_COLS 100 // Maximum number of map columns
#define MOVE_TIME 2 //Time to move

// Structure to store the map
typedef struct map {
    int sz_x; // Map size in x direction
    int sz_y; // Map size in y direction
    int map_data[MAX_ROWS][MAX_COLS]; // Map data matrix
} tMap;

// Structure to store a position
typedef struct pos {
    int x; // x coordinate
    int y; // y coordinate
} tPos;

// Structure to store a path consisting of multiple positions
typedef struct path {
    int num_coords; // Number of coordinates in the path
    tPos path[MAX_COORDS]; // Array of positions
} tPath;

// Function to read coordinates from a file and return them as a path
tPath read_coordinates(void);

// Function to read the map from a file and return it
tMap read_map_from_file(void);

// Function to get the map data as a 2D array
int **get_map(void);

// Function to get the next position based on the current position and map data
int get_next_pos(int **map_data, int *actual, int *next_pos);

#endif /* NAVIGATION_H */
