#include <stdio.h>
#include <stdlib.h>

typedef struct node_t {
    int n;
    struct node_t* next;
} *Node;

void destroyList(Node list) {
    if (list == NULL) return;
    destroyList(list->next);
    free(list);
}

/**
 * creates a node with given parameters as fields
 *
 * @param n - the data for the new node
 * @param next - the node that the created node will point to (can be NULL)
 * @return
 * NULL - if an allocation failed
 * a Node with wanted fields in any other case
 */
Node createNode(int n, Node next) {
    Node node = (Node)malloc(sizeof(*node));
    if (node == NULL) return NULL;
    node->n = n;
    node->next = next;
    return node;
}

Node listCopy(Node list) {
    if (list == NULL) return NULL;

    Node new_list = createNode(list->n, NULL);
    if (new_list == NULL) return NULL;
    Node last = new_list;
    list = list->next;

    while (list != NULL) {
        last->next = createNode(list->n, NULL);
        if (last->next == NULL) {
            destroyList(new_list);
            return NULL;
        }

        last = last->next;
        list = list->next;
    }
    return new_list;
}

Node listCopyReversed(Node list) {
    if (list == NULL) return NULL;
    Node copy = listCopy(list);
    if (copy == NULL) return NULL;

    Node reversed = NULL;
    while (copy != NULL) {
        reversed = createNode(copy->n, reversed);
        copy = copy->next;
    }
    return reversed;
}

Node listJoinAlternating(Node lists[], int size) {
    Node new_list = NULL;
    for (int i = 0; i < size; i++) {
        Node curr_list = NULL;

        if (i % 2 == 0) {
            curr_list = listCopyReversed(lists[i]);
        } else {
            curr_list = listCopy(lists[i]);
        }

        if (curr_list == NULL) {
            destroyList(new_list);
            return NULL;
        }

        if (new_list == NULL) {
            new_list = curr_list;
        } else {
            Node last = new_list;
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = curr_list;
        }
    }
    return new_list;
}

void print(Node list) {
    while (list != NULL) {
        printf("%d\n", list->n);
        list = list->next;
    }
}

int main() {
    Node list1 = createNode(1, createNode(2, NULL));
    Node list2 = createNode(3, createNode(4, NULL));
    Node list3 = createNode(5, createNode(6, NULL));
    Node list4 = createNode(7, createNode(8, NULL));
    Node lists[] = { list1, list2, list3, list4 };
    printf("first list:\n");
    print(list1);
    printf("second list:\n");
    print(list2);
    printf("third list:\n");
    print(list3);
    printf("fourth list:\n");
    print(list4);
    printf("alternating:\n");
    print(listJoinAlternating(lists, 4));
    getch();
}