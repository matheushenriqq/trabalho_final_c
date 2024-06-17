#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/list.h"

// Insert an element at the beginning of the list
void insert_list(tList *list, const char* new_data) {
    tNode* new_node = (tNode*) malloc(sizeof(tNode));
    if (new_node == NULL) {
        printf("Error: No space left in memory.\n");
        return;
    }

    // Allocate memory for the string and copy data
    new_node->data = strdup(new_data);
    if (new_node->data == NULL) {
        printf("Error: No space left in memory for string.\n");
        free(new_node);
        return;
    }

    new_node->next = list->first_elem; // Set next of new node
    list->first_elem = new_node; // Update list external pointer to new node
}

// Remove an element with the specified value
void remove_list(tList *list, const char* del_data) {
    if (list->first_elem == NULL) {
        printf("Cannot delete from an empty list.\n");
        return;
    }

    tNode* current = list->first_elem;
    tNode* prev = NULL;

    while (current != NULL && strcmp(current->data, del_data) != 0) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev == NULL) {
            list->first_elem = current->next;
        } else {
            prev->next = current->next;
        }

        free(current->data); // Free memory allocated for the string
        free(current);
    } else {
        printf("Node with data %s not found in the list.\n", del_data);
    }
}

// Release allocated memory for the entire list
void free_list(tList *list) {
    tNode* current = list->first_elem;
    tNode* temp;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp->data);  // Free memory allocated for the string
        free(temp);
    }

    list->first_elem = NULL; // Reset the list to empty
}

// Print all elements of the list
void print_list(tList *list) {
    tNode* current = list->first_elem;

    printf("List elements: ");
    while (current != NULL) {
        printf("%s ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Create and initialize a new list
tList create_list() {
    tList list;
    list.first_elem = NULL;  // Initialize the list with NULL pointer
    return list;
}
