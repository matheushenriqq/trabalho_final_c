#include "./include/navigation.h"
#include "./include/list.h"
#include "./include/info.h"
#include <stdio.h>
#include <stdlib.h>

// Function to clear the content of a file
void clear_file(const char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fclose(file);
    printf("File %s cleared.\n", filepath);
}

// Function to check if charging is impossible at the current position
void charge_impossible(int actual[2]) {
    tMap map = read_map_from_file();
    int **map_data = get_map();
    int y = actual[1];
    int x = actual[0];

    FILE *file = fopen("./data/charge_impossible.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing!\n");
        return;
    }

    // Check the current position and positions ahead
    int directions[5][2] = {
        {0, 1},  // North
        {1, 1},  // Northeast
        {-1, 1}, // Northwest
        {1, 0},  // East
        {-1, 0}  // West
    };

    for (int i = 0; i < 5; i++) {
        int new_x = x + directions[i][0];
        int new_y = y + directions[i][1];

        if (new_x >= 0 && new_x < map.sz_x && new_y >= 0 && new_y < map.sz_y) {
            int color = map.map_data[8 - new_y][new_x];
            if (color == 1) {
                printf("Checking sun at position (%d, %d): %d\n", new_x, new_y, color);
                fprintf(file, "impossibility of loading found at position (%d, %d)\nImpossible to charge\n", new_x, new_y);
                printf("impossibility of loading found at position (%d, %d) and written to file.\n", new_x, new_y);
            }
        }
    }

    fclose(file);
}

// Function to list the way taken by the robot
void list_way(tList *list, int actual[2]) {
    tMap map = read_map_from_file();
    int **map_data = get_map();
    int y = actual[1];
    int x = actual[0];

    int color = map.map_data[8 - y][x];
    switch (color) {
        case 255: insert_list(list, "free_2_go"); break;
        case 191: insert_list(list, "gold"); break;
        case 127: insert_list(list, "silver"); break;
        case 63:  insert_list(list, "bronze"); break;
        case 1:   insert_list(list, "charge_impossible"); break;  
    }

    tNode* current = list->first_elem;
    FILE *file = fopen("./data/list_way.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "List elements:\n");
    while (current != NULL) {
        fprintf(file, "%s\n", current->data);
        current = current->next;
    }

    fclose(file);
}

// Function to check for gold at the current position
int gold(int actual[2]) {
    tMap map = read_map_from_file();
    int **map_data = get_map();
    int y = actual[1];
    int x = actual[0];

    int count = 0;

    if (map.map_data[8 - y][x] == 191) {
        int color = map.map_data[8 - y][x];
        printf("Checking ground at position (%d, %d): %d\n", x, y, color);

        FILE *file = fopen("./data/gold.txt", "a");
        if (file == NULL) {
            fprintf(stderr, "Error opening file for writing!\n");
            return 0;
        }

        fprintf(file, "Gold found at position (%d, %d)\ngold\n", x, y);
        fclose(file);

        printf("Gold found at position (%d, %d) and written to file.\n", x, y);
        count++;
    }

    return count;
}

// Function to check for silver at the current position
int silver(int actual[2]) {
    tMap map = read_map_from_file();
    int **map_data = get_map();
    int x = actual[0];
    int y = actual[1];

    int count = 0;

    if (map.map_data[8 - y][x] == 127) {
        int color = map.map_data[8 - y][x];
        printf("Checking ground at position (%d, %d): %d\n", x, y, color);

        FILE *file = fopen("./data/silver.txt", "a");
        if (file == NULL) {
            fprintf(stderr, "Error opening file for writing!\n");
            return 0;
        }

        fprintf(file, "Silver found at position (%d, %d)\nsilver\n", x, y);
        fclose(file);

        printf("Silver found at position (%d, %d) and written to file.\n", x, y);
        count++;
    }

    return count;
}

// Function to check for bronze at the current position
int bronze(int actual[2]) {
    tMap map = read_map_from_file();
    int **map_data = get_map();
    int x = actual[0];
    int y = actual[1];

    int count = 0;

    if (map.map_data[8 - y][x] == 63) {
        int color = map.map_data[8 - y][x];
        printf("Checking ground at position (%d, %d): %d\n", x, y, color);

        FILE *file = fopen("./data/bronze.txt", "a");
        if (file == NULL) {
            fprintf(stderr, "Error opening file for writing!\n");
            return 0;
        }

        fprintf(file, "Bronze found at position (%d, %d)\nbronze\n", x, y);
        fclose(file);

        printf("Bronze found at position (%d, %d) and written to file.\n", x, y);
        count++;
    }

    return count;
}

// Function to check if a position already exists in the list of registered positions
int position_exists(Position *positions, int count, int x, int y) {
    for (int i = 0; i < count; i++) {
        if (positions[i].x == x && positions[i].y == y) {
            return 1;
        }
    }
    return 0;
}

// Function to identify obstacles and write their positions to a file
int obstacles(int actual[2], Position *registered_positions, int *registered_count) {
    tMap map = read_map_from_file();
    int **map_data = get_map();
    int y = actual[1];
    int x = actual[0];

    FILE *file = fopen("./data/obstacles.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing!\n");
        return 0;
    }

    int count = 0;

    // Check for obstacles (color 0) around the current position
    int directions[8][2] = {
        {0, 1},  // North
        {0, -1}, // South
        {1, 0},  // East
        {-1, 0}, // West
        {1, 1},  // Northeast
        {-1, -1},// Southwest
        {1, -1}, // Southeast
        {-1, 1}  // Northwest
    };

    for (int i = 0; i < 8; i++) {
        int new_x = x + directions[i][0];
        int new_y = y + directions[i][1];

        if (new_x >= 0 && new_x < map.sz_x && new_y >= 0 && new_y < map.sz_y) {
            int color = map.map_data[8 - new_y][new_x];
            if (color == 0 && !position_exists(registered_positions, *registered_count, new_x, new_y)) {
                registered_positions[*registered_count].x = new_x;
                registered_positions[*registered_count].y = new_y;
                (*registered_count)++;

                printf("obstacle_found (%d, %d)\n", new_x, new_y);
                fprintf(file, "Obstacle (color 0) found at position (%d, %d)\n", new_x, new_y);
                count++;
            }
        }
    }

    fclose(file);
    return count;
}
