#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "list_mtm.h" // ListElement, CopyListElement, FreeListElement
/**
 * Generic Node type
 *
 * A node has a pointed to data as well as a pointer to another node
 *
 * The following functions are available:
 *
 * nodeCreate       - Creates a node with the given data which points to the passed node
 * nodeDestroy      - Free the memory of the node and its data using the passed function
 *
 * nodeCopy         - Create a copy of the given node's data using the passed function and
 *                    return a node with the copied data in it
 * nodeSetNext      - Set the next node
 * nodeGetNext      - Get the next node
 * nodeGetData      - Get the data of the node
 */

/** Type for defining a node */
typedef struct Node_t *Node;

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
Node nodeCreate(ListElement data, Node next);

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
Node nodeCopy(Node node, CopyListElement copy_function, FreeListElement free_function);

/**
 * Set the next node
 *
 * parameters:
 * node - the node whose 'next' node will be changed
 * next - a node to be pointed to by 'node'
 */
void nodeSetNext(Node node, Node next);

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
Node nodeGetNext(Node node);

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
ListElement nodeGetData(Node node);

/**
 * Deallocate the memory of a node
 *
 * The data of the node is deallocated using the passed function
 * and then the memory of the node itself is freed
 *
 * parameters:
 * node - the node to destroy
 * free_function - the function used to free the memory of the data
 */
void nodeDestroy(Node node, FreeListElement free_function);

#endif //LIST_NODE_H
