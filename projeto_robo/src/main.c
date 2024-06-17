#include <stdio.h>
#include <stdlib.h>
#include "./include/navigation.h"
#include "./include/list.h"
#include "./include/battery.h"
#include "./include/info.h"

int main() {
    int total_time = 0;
    int total_gold = 0;
    int total_silver = 0;
    int total_bronze = 0;
    int **map_data = get_map();
    tList lista = create_list();
    Battery bateria;
    int actual[2] = {0, 0};
    int next_pos[2] = {-1, -1};
    int goal_position[2] = {7, 6}; //Last position at Path.txt
    

    // Clear previous data files
    clear_file("./data/bronze.txt");
    clear_file("./data/charge_impossible.txt");
    clear_file("./data/gold.txt");
    clear_file("./data/list_way.txt");
    clear_file("./data/silver.txt");
    clear_file("./data/charging.txt");
    clear_file("./data/obstacles.txt");

    // Initialize battery
    init_battery(&bateria);

    // Allocate memory for registered positions
    Position *registered_positions = malloc(sizeof(Position) * 100); // Assuming a maximum of 100 positions for simplicity
    int registered_count = 0;
    tPath path = read_coordinates();

    // Main loop for navigating the map
    for (int i = 0; i < path.num_coords - 1; i++){
    
        // Get the next position
        get_next_pos(map_data, actual, next_pos);

        // Check and generate files if necessary
        charge_impossible(actual);
        list_way(&lista, actual);
        obstacles(actual, registered_positions, &registered_count);
        total_gold += gold(actual);
        total_silver += silver(actual);
        total_bronze += bronze(actual);
        total_time += manage_battery_on_move(&bateria, actual, next_pos);

        // Update the current position
        actual[0] = next_pos[0];
        actual[1] = next_pos[1];
    }
    // Print the results
    printf("Total gold areas: %d\n", total_gold);
    printf("Total silver areas: %d\n", total_silver);
    printf("Total bronze areas: %d\n", total_bronze);
    printf("Final total time: %d seconds\n", total_time);
    if (actual[0] == goal_position[0] && actual[1] == goal_position[1])
        printf("sucess\n");
    else {
        printf ("failure");
    }


    // Free allocated memory
    free(registered_positions);
    free_list(&lista);
    free(map_data);
    return 0;
}
