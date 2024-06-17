#include "./include/navigation.h"
#include "./include/list.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_COORDS 1000 // Maximum number of coordinates
#define MAX_ROWS 100 // Maximum number of map rows
#define MAX_COLS 100 // Maximum number of map cols

// Function to read coordinates from a file and return them as a path
tPath read_coordinates(void) {
    tPath path;
    FILE *file = fopen("./data/path.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file!");
        exit(1);
    }

    // Read the number of coordinates from the first line
    fscanf(file, "%d", &path.num_coords);

    // Read each coordinate and store it in the array
    for (int i = 0; i < path.num_coords; i++) {
        fscanf(file, "%d, %d", &path.path[i].x, &path.path[i].y);
    }

    fclose(file);
    return path;
}

// Function to read the map from a file and return it
tMap read_map_from_file(void) {
    tMap map;
    FILE *file = fopen("./data/map.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file!");
        exit(1);
    }

    // Read the number of rows and columns
    fscanf(file, "%d %d", &map.sz_x, &map.sz_y);

    // Read the map data
    for (int i = 0; i < map.sz_x; i++) {
        for (int j = 0; j < map.sz_y; j++) {
            fscanf(file, "%d", &map.map_data[i][j]);
        }
    }

    fclose(file);
    return map;
}

// Function to get the map data as a 2D array
int **get_map() {
    tMap map = read_map_from_file();
    int **map_data = (int **)malloc(map.sz_x * sizeof(int *));
    if (map_data == NULL) {
        fprintf(stderr, "Error allocating memory for map data!");
        exit(1);
    }
    for (int i = 0; i < map.sz_x; i++) {
        map_data[i] = (int *)malloc(map.sz_y * sizeof(int));
        if (map_data[i] == NULL) {
            fprintf(stderr, "Error allocating memory for map data!");
            exit(1);
        }
        for (int j = 0; j < map.sz_y; j++) {
            map_data[i][j] = map.map_data[i][j];
        }
    }
    return map_data;
}

// Function to get the next position based on the current position and map data
int get_next_pos(int **map_data, int *actual, int *next_pos) {
    tPath path = read_coordinates();

    for (int i = 0; i < path.num_coords - 1; i++) {
        if (path.path[i].x == actual[0] && path.path[i].y == actual[1]) {
            next_pos[0] = path.path[i + 1].x;
            next_pos[1] = path.path[i + 1].y;
            return 0;
        }
    }

    fprintf(stderr, "Error! Unable to find next position!\n");
    next_pos[0] = -1;
    next_pos[1] = -1;
    return 0;
}

// Function to print the map
void print_map(void) {
    tMap map = read_map_from_file();
    printf("Size X: %d\n", map.sz_x);
    printf("Size Y: %d\n", map.sz_y);
    printf("Map Data:\n");
    for (int i = 0; i < map.sz_x; i++) {
        for (int j = 0; j < map.sz_y; j++) {
            printf("%d ", map.map_data[i][j]);
        }
        printf("\n");
    }
}
