#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "test_utilities.h"
#include "../list/node.h"

/**
 * Utility functions used in the tests
 */
static ListElement copyString(ListElement str) {
    char* copy = malloc(strlen(str) + 1);
    return copy ? strcpy(copy, str) : NULL;
}

static void freeString(ListElement str) {
    free(str);
}

/**
 * Since the Node ADT has only 3 functions, 1 test function can test all of them
 */
static bool testNode() {
    char* str = copyString("some text");
    Node node = nodeCreate(str, NULL);
    ASSERT_TEST(node != NULL);

    ASSERT_TEST(nodeCopy(NULL, NULL, NULL) == NULL);
    ASSERT_TEST(nodeCopy(NULL, copyString, freeString) == NULL);
    ASSERT_TEST(nodeCopy(node, NULL, freeString) == NULL);
    ASSERT_TEST(nodeCopy(node, copyString, NULL) == NULL);
    Node copied_node = nodeCopy(node, copyString, freeString);
    ASSERT_TEST(copied_node != NULL);

    nodeSetNext(node, copied_node);
    nodeSetNext(copied_node, node);

    ASSERT_TEST(nodeGetNext(NULL) == NULL);
    ASSERT_TEST(nodeGetNext(node) == copied_node);
    ASSERT_TEST(nodeGetNext(copied_node) == node);

    ASSERT_TEST(strcmp((char*) nodeGetData(node), "SOME TEXT"));
    ASSERT_TEST(!strcmp((char*) nodeGetData(node), "some text"));
    ASSERT_TEST(strcmp((char*) nodeGetData(copied_node), "not some text"));
    ASSERT_TEST(!strcmp((char*) nodeGetData(copied_node), "some text"));

    nodeDestroy(node, freeString);
    nodeDestroy(copied_node, freeString);

    return true;
}

int runNodeTests() {
    RUN_TEST(testNode);

    return 0;
}