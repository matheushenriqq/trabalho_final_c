#ifndef LIST_H
#define LIST_H

// Structure to represent a node in the list
typedef struct node {
    char* data;
    struct node* next;
} tNode;

// Structure to represent the list
typedef struct list {
    tNode* first_elem;
} tList;

// Function to list the way taken by the robot
void list_way(tList *list, int actual[2]);

// Function to insert a new element into the list
void insert_list(tList *list, const char* new_data);

// Function to remove an element from the list
void remove_list(tList *list, const char* del_data);

// Function to free the memory allocated for the list
void free_list(tList *list);

// Function to print all elements in the list
void print_list(tList *list);

// Function to create and initialize a new list
tList create_list();

#endif /* LIST_H */
