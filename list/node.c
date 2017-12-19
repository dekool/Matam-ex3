#include <stdlib.h>
#include "node.h"

/** Type for defining a node */
struct Node_t {
    ListElement data;
    struct Node_t* next;
};

/**
 * Create a new node
 *
 * Creates a new node with given data and next node to point to
 *
 * parameters:
 * data - a variable of type ListElement that will be the data of the created node
 * next - a variable of type Node which will be pointed by the created node
 *
 * return value:
 * NULL - if an allocation failed
 * A new node in case of success
 */
Node nodeCreate(ListElement data, Node next) {
    Node new_node = (Node) malloc(sizeof(*new_node));
    if(new_node == NULL) return NULL;
    new_node->data = data;
    new_node->next = (Node) next;
    return new_node;
}

/**
 * Creates a copy of the given node
 *
 * The new node will contain a copy of the given node's data by using the passed copy function
 * The new node will point to NULL as its next node
 *
 * parameters:
 * node - the node to copy
 * copy_function - a function used to copy the given node's data
 * free_function - a function used to free the data in case of a failure
 *
 * return value:
 * NULL - if a given parameter is NULL or an allocation failed
 * The copied node in the case of success
 */
Node nodeCopy(Node node, CopyListElement copy_function, FreeListElement free_function) {
    if(!node || !copy_function || !free_function) return NULL;

    ListElement copied_data = copy_function(node->data);
    if(copied_data == NULL) return NULL;

    Node new_node = nodeCreate(copied_data, NULL);
    if(new_node == NULL) {
        free_function(copied_data);
        return NULL;
    }

    new_node->data = copied_data;
    new_node->next = NULL;
    return new_node;
}

/**
 * Set the next node pointed by a node
 *
 * parameters:
 * node - the node whose 'next' node will be changed
 * next - a node to be pointed to by 'node'
 *
 * - if the passed node is NULL, then nothing is changed
 */
void nodeSetNext(Node node, Node next) {
    if(node == NULL) return;
    node->next = (Node) next;
}

/**
 * Get the next node
 *
 * parameters:
 * node - the node whose 'next' node will be returned
 *
 * return value:
 * NULL - if the passed node is NULL
 * the next node in any other case
 */
Node nodeGetNext(Node node) {
    if(node == NULL) return NULL;
    return node->next;
}

/**
 * Get the data of the node
 *
 * parameters:
 * node - the node whose data will be returned
 *
 * return value:
 * NULL - if the passed node is NULL
 * the data of the node in any other case
 */
ListElement nodeGetData(Node node) {
    if(node == NULL) return NULL;
    return node->data;
}

/**
 * Deallocate the memory of a node
 *
 * The data of the node will be deallocated using the passed function
 * and then the memory of the node itself will be freed
 *
 * parameters:
 * node - the node to destroy
 * free_function - the function used to free the memory of the data
 */
void nodeDestroy(Node node, FreeListElement free_function) {
    free_function(node->data);
    free(node);
}