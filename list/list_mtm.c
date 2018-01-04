#include <stdlib.h>

#include "list_mtm.h"

// ================== Node ADT Implementation =======================
/** Type for defining a node */
typedef struct Node_t {
    ListElement data;
    struct Node_t* next;
} *Node;

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
// ================== End of Node ADT Implementation ================

// ================== List ADT Implementation =======================
/** Type for defining the list */
struct List_t {
    CopyListElement copy_function;
    FreeListElement free_function;
    Node head;
    Node iterator;
};

/**
 * Allocates a new List.
 *
 * Creates a new empty list. This function receives the functions which will be
 * used for copying elements into the list and freeing them when needed.
 * For example, in order to create a list of strings one need to define these
 * two functions:
 * @code
 * ListElement copyString(ListElement str);
 * void freeString(ListElement str);
 * @endcode
 * And then create a list as follows:
 * @code
 * List listOfStrings = listCreate(copyString, freeString);
 * @endcode
 *
 * The iterator of the new list should point to NULL.
 *
 * @param copyElement Function pointer to be used for copying elements into
 * the list or when copying the list.
 * @param freeElement Function pointer to be used for removing elements from
 * the list.
 * @return
 * NULL - if one of the parameters is NULL or allocations failed.
 * A new List in case of success.
 */
List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    if(copyElement == NULL || freeElement == NULL) return NULL;
    List list = (List) malloc(sizeof(*list));
    if(list == NULL) return NULL;
    list->copy_function = copyElement;
    list->free_function = freeElement;
    list->head = NULL;
    list->iterator = NULL;
    return list;
}

/**
 * Creates a copy of target list.
 *
 * The new copy will contain all the elements from the source list in the same
 * order and will use the same functions as the original list for copying and
 * freeing elements.
 *
 * The iterator of the new list should point to the same element that the
 * original iterator is pointing to in the original list.
 *
 * @param list The target list to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A List containing the same elements with same order as list otherwise.
 */
List listCopy(List list) {
    if(list == NULL) return NULL;
    List new_list = listCreate(list->copy_function, list->free_function);
    if(new_list == NULL) return NULL;
    Node original_iterator = list->iterator;
    // copy the nodes
    for(ListElement element = listGetFirst(list); element != NULL; element = listGetNext(list)) {
        ListElement data_copy = list->copy_function(element);
        if(data_copy == NULL) {
            listDestroy(new_list);
            return NULL;
        }
        if(listInsertLast(new_list, data_copy) != LIST_SUCCESS) {
            list->free_function(data_copy);
            listDestroy(new_list);
            return NULL;
        }
    }
    list->iterator = original_iterator;
    for(Node node = list->head, copy_node = new_list->head;
        node != NULL;
        node = nodeGetNext(node), copy_node = nodeGetNext(copy_node)) {

        if(list->iterator == node) {
            new_list->iterator = copy_node;
        }
    }
    return new_list;
}

/**
 * Returns the number of elements in a list
 *
 * The iterator should not change.
 *
 * @param list The target list which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the list.
 */
int listGetSize(List list) {
    if(list == NULL) return -1;
    int size = 0;
    for(Node node = list->head; node != NULL; node = nodeGetNext(node)) {
        size++;
    }
    return size;
}

/**
 * Sets the internal iterator to the first element and retrieves it.
 *
 * The list has an internal iterator (also called current element) to allow
 * iteration over the list's elements. This function sets the iterator to point
 * to the first element in the list and return it.
 * Use this to start iteraing over the list, searching from the beginning of
 * the list and/or get the first element in the list.
 * (To continue iteration use listGetNext)
 * @code
 * void f(List list) {
 *   ListElement first = listGetFirst(list);
 *   printf("The first element is at address %x\n", first);
 * }
 * @endcode
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * @param list The list for which to set the iterator and return the first
 * element.
 * @return
 * NULL is a NULL pointer was sent or the list is empty.
 * The first element of the list otherwise
 */
ListElement listGetFirst(List list) {
    if(list == NULL) return NULL;
    list->iterator = list->head;
    return nodeGetData(list->iterator);
}

/**
 * Advances the list's iterator to the next element and returns it. If the
 * iterator points to the last element, the iterator should be set to NULL and
 * returned.
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * @param list The list for which to advance the iterator
 * @return
 * NULL if reached the end of the list, the iterator points to NULL or a NULL
 * sent as argument
 * The next element on the list in case of success
 */
ListElement listGetNext(List list) {
    if(list == NULL) return NULL;
    if(list->iterator == NULL) return NULL;

    list->iterator = nodeGetNext(list->iterator);
    return nodeGetData(list->iterator);
}

/**
 * Returns the current element (pointed by the iterator).
 *
 * Note: the element which is stored in the list is returned, not a copy.
 *
 * The iterator should not change.
 *
 * @param list The list for which to get the iterator
 * @return
 * NULL if the iterator points to NULL or a NULL sent as argument
 * The current element on the list in case of success
 */
ListElement listGetCurrent(List list) {
    if(list == NULL) return NULL;
    return nodeGetData(list->iterator);
}

/**
 * Adds a new element to the list, the new element will be the first element.
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertFirst(List list, ListElement element) {
    if(list == NULL || element == NULL) return LIST_NULL_ARGUMENT;
    Node new_node = nodeCreate(element, list->head);
    if(new_node == NULL) return LIST_OUT_OF_MEMORY;
    nodeSetNext(new_node, list->head);
    list->head = new_node;
    return LIST_SUCCESS;
}

/**
 * Adds a new element to the list, the new element will be the last element
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertLast(List list, ListElement element) {
    if(list == NULL || element == NULL) return LIST_NULL_ARGUMENT;

    if(list->head == NULL) { // if the list is empty
        return listInsertFirst(list, element);
    }

    Node new_node = nodeCreate(element, NULL);
    if(new_node == NULL) return LIST_OUT_OF_MEMORY;
    // get the last node in the list
    Node last_node = NULL;
    for(last_node = list->head; nodeGetNext(last_node) != NULL; last_node = nodeGetNext(last_node));
    nodeSetNext(last_node, new_node);

    return LIST_SUCCESS;
}

/**
 * Returns the node which is before the current node pointed by the list's iterator
 *
 * The iterator is not changed
 *
 * parameters:
 * list - the list whose node will be returned
 *
 * return value:
 * NULL - if the passed list is NULL or if the iterator points to the first node
 * the actual node in any other case
 */
static Node listGetBefore(List list) {
    if(list == NULL) return NULL;
    if(list->iterator == list->head) return NULL;

    Node before = NULL;
    for(before = list->head;
        nodeGetNext(before) != list->iterator && before != NULL;
        before = nodeGetNext(before));
    return before;
}

/**
 * Adds a new element to the list, the new element will be place right before
 * the current element (as pointed by the inner iterator of the list)
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element before its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_INVALID_CURRENT if the list's iterator points to NULL
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertBeforeCurrent(List list, ListElement element) {
    if(list == NULL || element == NULL) return LIST_NULL_ARGUMENT;
    if(list->iterator == NULL) return LIST_INVALID_CURRENT;

    Node new_node = nodeCreate(element, list->iterator);
    if(new_node == NULL) return LIST_OUT_OF_MEMORY;

    if(list->iterator == list->head) {
        list->head = new_node;
    } else {
        // get the node before the current one
        Node before_current = listGetBefore(list);
        if(before_current != NULL) {
            nodeSetNext(before_current, new_node);
        }
    }
    return LIST_SUCCESS;
}

/**
 * Adds a new element to the list, the new element will be place right after
 * the current element (as pointed by the inner iterator be of the list)
 *
 * The iterator should not change.
 *
 * @param list The list for which to add an element after its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * LIST_INVALID_CURRENT if the list's iterator points to NULL
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertAfterCurrent(List list, ListElement element) {
    if(list == NULL || element == NULL) return LIST_NULL_ARGUMENT;
    if(list->iterator == NULL) return LIST_INVALID_CURRENT;

    Node new_node = nodeCreate(element, nodeGetNext(list->iterator));
    if(new_node == NULL) return LIST_OUT_OF_MEMORY;

    nodeSetNext(list->iterator, new_node);
    return LIST_SUCCESS;
}

/**
 * Removes the currently pointed element of the list using the stored freeing
 * function
 *
 * The iterator should be set to NULL.
 *
 * @param list The list for which the current element will be removed
 * @return
 * LIST_NULL_ARGUMENT if list is NULL
 * LIST_INVALID_CURRENT if the current pointer of the list points to NULL
 * LIST_SUCCESS the current element was removed successfully
 */
ListResult listRemoveCurrent(List list) {
    if(list == NULL) return LIST_NULL_ARGUMENT;
    if(list->iterator == NULL) return LIST_INVALID_CURRENT;

    if(list->iterator == list->head) {
        list->head = nodeGetNext(list->iterator);
    } else {
        Node before = listGetBefore(list);
        nodeSetNext(before, nodeGetNext(list->iterator));
    }
    nodeDestroy(list->iterator, list->free_function);
    list->iterator = NULL;
    return LIST_SUCCESS;
}

/**
 * Perform one iteration of bubble sort on the list
 *
 * If any of the parameters are illegal, then the list is not changed
 *
 * @param list - the list to perform the bubbling on
 * @param compareElement - a function used to compare 2 list elements
 * @param key - extra information needed for the comparison
 */
static void listBubble(List list, CompareListElements compareElement, ListSortKey key) {
    if(list == NULL || compareElement == NULL) return;

    for(Node node = list->head; nodeGetNext(node) != NULL; node = nodeGetNext(node)) {
        Node node1 = node;
        Node node2 = nodeGetNext(node1);
        Node after = nodeGetNext(node2);
        ListElement data1 = nodeGetData(node1);
        ListElement data2 = nodeGetData(node2);
        if(compareElement(data1, data2, key) > 0) { // need to swap
            if(node1 == list->head) {
                list->head = node2;
                nodeSetNext(node2, node1);
                nodeSetNext(node1, after);
            } else {
                list->iterator = node;
                Node before = listGetBefore(list);
                nodeSetNext(before, node2);
                nodeSetNext(node2, node1);
                nodeSetNext(node1, after);
            }
        }
    }
}

/**
 * Sorts the list according to the given function.
 *
 * For example, the following code will sort a list of integers according to
 * their distance from 0.
 * @code
 * int closerTo(ListElement num1, ListElement num2, ListSortKey value) {
 *   int distance1 = abs(*(int*)num1 - *(int*)value);
 *   int distance2 = abs(*(int*)num2 - *(int*)value);
 *   return distance1 - distance2;
 * }
 *
 * void sortInts(List listOfInts) {
 *   int key = 0;
 *   listSort(listOfInts, closerTo, &key);
 * }
 * @endcode
 *
 * Elements which are treated as equal by the comparison function don't have a
 * defined order.
 *
 * After sorting, the iterator points to the same node in order. That is, if the
 * list before sorting was (1, 5, 2, 6), and the iterator pointed to "5", after
 * the sorting, (1, 2, 5, 6), the iterator will still point to the second
 * element in the list, which is "2".
 *
 * @param list the target list to sort
 * @param compareElement A comparison function as defined in the type
 * CompareListElements. This function should return an integer indicating the
 * relation between two elements in the list
 *
 * @return
 * LIST_NULL_ARGUMENT if list or compareElement are NULL
 * LIST_OUT_OF_MEMORY if a memory allocation failed, the list will be intact
 * in this case.
 * LIST_SUCCESS if sorting completed successfully.
 */
ListResult listSort(List list, CompareListElements compareElement, ListSortKey key) {
    if(list == NULL || compareElement == NULL) return LIST_NULL_ARGUMENT;

    List new_list = listCopy(list);
    if(new_list == NULL) return LIST_OUT_OF_MEMORY;
    Node original_iterator = list->iterator;

    // bubble sort
    for(int i = 0; i < listGetSize(new_list); i++) {
        listBubble(new_list, compareElement, key);
    }

    for(Node original_node = list->head, new_node = new_list->head;
        original_node != NULL;
        original_node = nodeGetNext(original_node), new_node = nodeGetNext(new_node)) {

        if(original_node == list->iterator) {
            new_list->iterator = new_node;
        }
    }
    list->iterator = original_iterator;

    listClear(list);

    list->head = new_list->head;
    list->iterator = new_list->iterator;
    free(new_list);
    return LIST_SUCCESS;
}

/**
 * Creates a new filtered copy of a list.
 *
 * This creates a new list with only the elements for which the filtering
 * function returned true.
 *
 * For example, the following code creates a new list, given a list of strings
 * containing only the strings which are longer than 10 characters.
 * @code
 *
 * bool isLongerThan(ListElement string, ListFilterKey key) {
 *   return strlen(string) > *(int*)key;
 * }
 *
 * List createFilteredList(List listOfStrings) {
 *   int key = 10;
 *   return listFilter(listOfStrings, isLongerThan, &key);
 * }
 * @endcode
 *
 * The iterator of the new list should point to NULL. The iterator of the
 * source list should not change.
 *
 * @param list The list for which a filtered copy will be made
 * @param filterElement The function used for determining whether a given
 * element should be in the resulting list or not.
 * @param key Any extra values that need to be sent to the filtering function
 * when called
 * @return
 * NULL if list or filterElement are NULL or a memory allocation failed.
 * A List containing only elements from list which filterElement returned true
 * for.
 */
List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    if(list == NULL || filterElement == NULL) return NULL;

    List new_list = listCreate(list->copy_function, list->free_function);
    for(Node node = list->head; node != NULL; node = nodeGetNext(node)) {
        ListElement data = nodeGetData(node);
        if(filterElement(data, key)) {
            ListElement data_copy = list->copy_function(data);
            if(data_copy == NULL) {
                listDestroy(new_list);
                return NULL;
            }
            if(listInsertLast(new_list, data_copy) != LIST_SUCCESS) {
                list->free_function(data_copy);
                listDestroy(new_list);
                return NULL;
            }
        }
    }
    new_list->iterator = NULL;
    return new_list;
}

/**
 * Removes all elements from target list.
 * The elements are deallocated using the stored freeing function.
 *
 * The iterator should be set to NULL.
 *
 * @param list Target list to remove all element from
 * @return
 * LIST_NULL_ARGUMENT - if a NULL pointer was sent.
 * LIST_SUCCESS - Otherwise.
 */
ListResult listClear(List list) {
    if(list == NULL) return LIST_NULL_ARGUMENT;

    while(listGetSize(list) > 0) {
        listGetFirst(list);     // make the iterator point to the first node in the list
        listRemoveCurrent(list);
    }
    list->iterator = NULL;

    return LIST_SUCCESS;
}

/**
 * Deallocates an existing list. Clears all elements by using the stored free
 * function.
 *
 * @param list Target list to be deallocated. If list is NULL nothing will be
 * done
 */
void listDestroy(List list) {
    if(list == NULL) return;
    listClear(list);
    free(list);
}
// ================= End of List ADT Implementation ======================