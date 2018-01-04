#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "test_utilities.h"
#include "../list_mtm/list_mtm.h"

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
 * List of tests for List
 */
static bool testListCreate() {
    ASSERT_TEST(listCreate(NULL, NULL) == NULL);
    ASSERT_TEST(listCreate(copyString, NULL) == NULL);
    ASSERT_TEST(listCreate(NULL, freeString) == NULL);

    List test_list = listCreate(copyString, freeString);
    bool valid_list = test_list != NULL;
    listDestroy(test_list);
    ASSERT_TEST(valid_list);

    return true;
}

static bool testListCopy() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listCopy(NULL) == NULL);

    ASSERT_TEST(listInsertFirst(list, copyString("some text")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("abcde")) == LIST_SUCCESS);
    listGetFirst(list);     // reset the iterator to point to the first element
    ASSERT_TEST(listInsertBeforeCurrent(list, copyString("TEXT")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(list, copyString("fghijk")) == LIST_SUCCESS);

    // current list content: ["TEXT", "some text", "fghijk", "abcde"]
    List copy = listCopy(list);
    ASSERT_TEST(copy != NULL);
    ASSERT_TEST(listGetSize(copy) == 4);
    ASSERT_TEST(!strcmp((char*) listGetFirst(copy), "TEXT"));
    ASSERT_TEST(!strcmp((char*) listGetNext(copy), "some text"));
    ASSERT_TEST(!strcmp((char*) listGetNext(copy), "fghijk"));
    ASSERT_TEST(!strcmp((char*) listGetNext(copy), "abcde"));
    ASSERT_TEST(listGetNext(copy) == NULL);

    listDestroy(list);

    ASSERT_TEST(copy != NULL);
    ASSERT_TEST(listGetSize(copy) == 4);
    ASSERT_TEST(!strcmp((char*) listGetFirst(copy), "TEXT"));
    ASSERT_TEST(!strcmp((char*) listGetNext(copy), "some text"));
    ASSERT_TEST(!strcmp((char*) listGetNext(copy), "fghijk"));
    ASSERT_TEST(!strcmp((char*) listGetNext(copy), "abcde"));
    ASSERT_TEST(listGetNext(copy) == NULL);
    listDestroy(copy);

    return true;
}

static bool testListGetSize() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listInsertFirst(list, copyString("some text")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 1);
    ASSERT_TEST(listInsertLast(list, copyString("abcde")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 2);

    ASSERT_TEST(!strcmp(listGetFirst(list), "some text"));
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 1);

    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "abcde"));
    ASSERT_TEST(listInsertBeforeCurrent(list, copyString("TEXT")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 2);

    ASSERT_TEST(listInsertAfterCurrent(list, copyString("fghijk")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 3);

    // current list: ["TEXT", "abcde", "fghijk"]

    ASSERT_TEST(!strcmp((char*) listGetNext(list), "fghijk"));
    ASSERT_TEST(listGetNext(list) == NULL);

    listDestroy(list);
    return true;
}

static bool testListInsertFirst() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listInsertFirst(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(NULL, "some text") == LIST_NULL_ARGUMENT);

    ASSERT_TEST(listInsertFirst(list, copyString("a a a")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, copyString("b b b")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list, copyString("c c c")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 3);

    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "c c c"));
    ASSERT_TEST(!strcmp((char*) listGetNext(list), "b b b"));
    ASSERT_TEST(!strcmp((char*) listGetNext(list), "a a a"));

    listDestroy(list);
    return true;
}

static bool testListInsertLast() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listInsertLast(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertLast(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertLast(NULL, "some text") == LIST_NULL_ARGUMENT);

    ASSERT_TEST(listInsertLast(list, copyString("a a a")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("b b b")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("c c c")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 3);

    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "a a a"));
    ASSERT_TEST(!strcmp((char*) listGetNext(list), "b b b"));
    ASSERT_TEST(!strcmp((char*) listGetNext(list), "c c c"));

    listDestroy(list);
    return true;
}

static bool testListInsertBeforeCurrent() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listInsertBeforeCurrent(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(NULL, "abcd") == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(list, "abcd") == LIST_INVALID_CURRENT);

    ASSERT_TEST(listInsertFirst(list, copyString("first")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list, "abcd") == LIST_INVALID_CURRENT);

    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "first"));
    ASSERT_TEST(listInsertBeforeCurrent(list, copyString("abcd")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 2);
    ASSERT_TEST(listInsertBeforeCurrent(list, copyString("efgh")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 3);

    // current list: ["abcd", "efgh", "first"]
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "abcd"));
    ASSERT_TEST(!strcmp((char*) listGetNext(list), "efgh"));
    ASSERT_TEST(!strcmp((char*) listGetNext(list), "first"));
    ASSERT_TEST(listGetNext(list) == NULL);

    listDestroy(list);
    return true;
}

static bool testListInsertAfterCurrent() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listInsertAfterCurrent(NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(list, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(NULL, "abcd") == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(list, "abcd") == LIST_INVALID_CURRENT);

    ASSERT_TEST(listInsertFirst(list, copyString("first")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(list, "abcd") == LIST_INVALID_CURRENT);

    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "first"));
    ASSERT_TEST(listInsertAfterCurrent(list, copyString("abcd")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 2);
    ASSERT_TEST(listInsertAfterCurrent(list, copyString("efgh")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 3);

    // current list: ["first", "efgh", "abcd"]
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "first"));
    ASSERT_TEST(!strcmp((char*) listGetNext(list), "efgh"));
    ASSERT_TEST(!strcmp((char*) listGetNext(list), "abcd"));
    ASSERT_TEST(listGetNext(list) == NULL);

    listDestroy(list);
    return true;
}

static bool testListRemoveCurrent() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);

    ASSERT_TEST(listInsertFirst(list, copyString("first")) == LIST_SUCCESS);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "first"));
    ASSERT_TEST(listGetSize(list) == 1);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 0);

    ASSERT_TEST(listInsertLast(list, copyString("first")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("second")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("third")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 3);
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "first"));
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 2);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "second"));
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 1);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);

    listDestroy(list);
    return true;
}

static bool testListGetCurrent() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listGetCurrent(NULL) == NULL);
    ASSERT_TEST(listGetCurrent(list) == NULL);

    ASSERT_TEST(listInsertLast(list, copyString("abcd")) == LIST_SUCCESS);
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "abcd"));
    ASSERT_TEST(!strcmp((char*) listGetCurrent(list), "abcd"));

    ASSERT_TEST(listInsertLast(list, copyString("efgh")) == LIST_SUCCESS);
    ASSERT_TEST(!strcmp((char*) listGetCurrent(list), "abcd"));

    ASSERT_TEST(!strcmp((char*) listGetNext(list), "efgh"));
    ASSERT_TEST(!strcmp((char*) listGetCurrent(list), "efgh"));
    ASSERT_TEST(listGetNext(list) == NULL);
    ASSERT_TEST(listGetCurrent(list) == NULL);

    listDestroy(list);
    return true;
}

static bool testListGetFirst() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listGetFirst(NULL) == NULL);
    ASSERT_TEST(listGetFirst(list) == NULL);

    ASSERT_TEST(listInsertFirst(list, copyString("abcd")) == LIST_SUCCESS);
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "abcd"));
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "abcd"));
    ASSERT_TEST(!strcmp((char*) listGetCurrent(list), "abcd"));

    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetFirst(list) == NULL);
    ASSERT_TEST(listGetCurrent(list) == NULL);

    listDestroy(list);
    return true;
}

static bool testListGetNext() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listGetNext(NULL) == NULL);
    ASSERT_TEST(listGetNext(list) == NULL);

    ASSERT_TEST(listInsertLast(list, copyString("a a a")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("b b b")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("c c c")) == LIST_SUCCESS);

    ASSERT_TEST(!strcmp(listGetFirst(list), "a a a"));
    ASSERT_TEST(!strcmp(listGetNext(list), "b b b"));
    ASSERT_TEST(!strcmp(listGetFirst(list), "a a a"));
    ASSERT_TEST(!strcmp(listGetNext(list), "b b b"));
    ASSERT_TEST(!strcmp(listGetNext(list), "c c c"));
    ASSERT_TEST(listGetNext(list) == NULL);

    listDestroy(list);
    return true;
}

static int sortingFunction(char* str1, char* str2, ListSortKey key) {
    return strcmp(str1, str2);
}

static bool testListSort() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listSort(NULL, NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listSort(list, NULL, NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listSort(NULL, (CompareListElements) sortingFunction, NULL) == LIST_NULL_ARGUMENT);

    List sorted = listCopy(list);
    ASSERT_TEST(sorted != NULL);
    ASSERT_TEST(listSort(sorted, (CompareListElements) sortingFunction, NULL) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(sorted) == 0);
    ASSERT_TEST(listGetCurrent(sorted) == NULL);
    listDestroy(sorted);

    ASSERT_TEST(listInsertLast(list, copyString("aaa")) == LIST_SUCCESS);
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "aaa"));

    sorted = listCopy(list);
    ASSERT_TEST(sorted != NULL);
    ASSERT_TEST(listSort(sorted, (CompareListElements) sortingFunction, NULL) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(sorted) == 1);
    ASSERT_TEST(!strcmp((char*) listGetCurrent(sorted), "aaa"));
    listDestroy(sorted);

    ASSERT_TEST(listInsertLast(list, copyString("mtm")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("abc")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("Technion")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("AAA")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("abc")) == LIST_SUCCESS);
    ASSERT_TEST(listSort(list, NULL, NULL) == LIST_NULL_ARGUMENT);
    // current list: ["aaa", "mtm", "abc", "Technion", "AAA", "abc"]

    ASSERT_TEST(!strcmp((char*) listGetNext(list), "mtm"));
    sorted = listCopy(list);
    ASSERT_TEST(sorted != NULL);
    ASSERT_TEST(listSort(sorted, (CompareListElements) sortingFunction, NULL) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(sorted) == 6);
    ASSERT_TEST(!strcmp((char*) listGetCurrent(sorted), "Technion"));
    ASSERT_TEST(!strcmp((char*) listGetNext(sorted), "aaa"));
    ASSERT_TEST(!strcmp((char*) listGetNext(sorted), "abc"));
    ASSERT_TEST(!strcmp((char*) listGetNext(sorted), "abc"));
    ASSERT_TEST(!strcmp((char*) listGetNext(sorted), "mtm"));
    ASSERT_TEST(listGetNext(sorted) == NULL);

    listDestroy(sorted);
    listDestroy(list);
    return true;
}

// check if 'element' starts with the character pointed to by 'key'
static bool filteringFunction(char* element, char* key) {
    return element[0] == *key;
}

static bool testListFilter() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listFilter(NULL, NULL, NULL) == NULL);
    ASSERT_TEST(listFilter(list, NULL, NULL) == NULL);
    ASSERT_TEST(listFilter(NULL, (FilterListElement) filteringFunction, NULL) == NULL);

    char key = 'a';
    List filtered = listFilter(list, (FilterListElement) filteringFunction, &key);
    ASSERT_TEST(filtered != NULL);
    ASSERT_TEST(listGetSize(filtered) == 0);
    ASSERT_TEST(listGetCurrent(filtered) == NULL);
    listDestroy(filtered);

    ASSERT_TEST(listInsertLast(list, copyString("bbb")) == LIST_SUCCESS);
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "bbb"));

    filtered = listFilter(list, (FilterListElement) filteringFunction, &key);
    ASSERT_TEST(filtered != NULL);
    ASSERT_TEST(listGetSize(filtered) == 0);
    ASSERT_TEST(listGetCurrent(filtered) == NULL);
    listDestroy(filtered);

    key = 'b';
    filtered = listFilter(list, (FilterListElement) filteringFunction, &key);
    ASSERT_TEST(filtered != NULL);
    ASSERT_TEST(listGetSize(filtered) == 1);
    ASSERT_TEST(listGetCurrent(filtered) == NULL);
    ASSERT_TEST(!strcmp((char*) listGetFirst(filtered), "bbb"));
    ASSERT_TEST(listGetNext(filtered) == NULL);
    listDestroy(filtered);

    ASSERT_TEST(listInsertLast(list, copyString("mtm")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("boy")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("bbb")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("some text")) == LIST_SUCCESS);

    // current list: ["bbb", "mtm", "boy", "bbb", "some text"]
    ASSERT_TEST(listGetSize(list) == 5);
    filtered = listFilter(list, (FilterListElement) filteringFunction, &key);
    // filtered list: ["bbb", "boy", "bbb"]
    ASSERT_TEST(filtered != NULL);
    ASSERT_TEST(listGetSize(filtered) == 3);

    ASSERT_TEST(!strcmp((char*) listGetFirst(filtered), "bbb"));
    ASSERT_TEST(!strcmp((char*) listGetNext(filtered), "boy"));
    ASSERT_TEST(!strcmp((char*) listGetNext(filtered), "bbb"));
    ASSERT_TEST(listGetNext(filtered) == NULL);
    listDestroy(filtered);

    listDestroy(list);
    return true;
}

static bool testListClear() {
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(list != NULL);

    ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);

    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 0);

    ASSERT_TEST(listInsertLast(list, copyString("abcd")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 1);
    ASSERT_TEST(!strcmp((char*) listGetFirst(list), "abcd"));
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 0);
    ASSERT_TEST(listGetCurrent(list) == NULL);

    ASSERT_TEST(listInsertLast(list, copyString("a")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("b")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("c")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("d")) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list, copyString("e")) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 5);
    for(ListElement element = listGetFirst(list); element != NULL; element = listGetNext(list));
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);

    listDestroy(list);
    return true;
}

int main() {
    RUN_TEST(testListCreate);
    RUN_TEST(testListCopy);
    RUN_TEST(testListGetSize);
    RUN_TEST(testListInsertFirst);
    RUN_TEST(testListInsertLast);
    RUN_TEST(testListInsertBeforeCurrent);
    RUN_TEST(testListInsertAfterCurrent);
    RUN_TEST(testListRemoveCurrent);
    RUN_TEST(testListGetCurrent);
    RUN_TEST(testListGetFirst);
    RUN_TEST(testListGetNext);
    RUN_TEST(testListSort);
    RUN_TEST(testListFilter);
    RUN_TEST(testListClear);

    return 0;
}