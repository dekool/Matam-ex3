#include "test_utilities.h"
#include "../grade.h"
#include "stdio.h"

static bool testGradeCreate() {
    // GradeResult gradeCreate(int course_id, char* points, int grade_number,int semester_number, Grade *grade);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(getGradeNumber(grade_test) == 95);
    ASSERT_TEST(getCoursePointsX2(grade_test) == 7);
    ASSERT_TEST(getCourseId(grade_test) == 11071);
    ASSERT_TEST(getSemester(grade_test) == 1);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.0", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(getGradeNumber(grade_test) == 95);
    ASSERT_TEST(getCoursePointsX2(grade_test) == 6);
    ASSERT_TEST(getCourseId(grade_test) == 11071);
    ASSERT_TEST(getSemester(grade_test) == 1);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(getGradeNumber(grade_test) == 95);
    ASSERT_TEST(getCoursePointsX2(grade_test) == 6);
    ASSERT_TEST(getCourseId(grade_test) == 11071);
    ASSERT_TEST(getSemester(grade_test) == 1);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, NULL, 95, 1, &grade_test) == GRADE_NULL_ARGUMENT);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(-2, "3.5", 95, 1, &grade_test) == GRADE_INVALID_PARAMETER);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(2000000000, "3.5", 95, 1, &grade_test) == GRADE_INVALID_PARAMETER);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.55", 95, 1, &grade_test) == GRADE_INVALID_PARAMETER);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "-1.5", 95, 1, &grade_test) == GRADE_INVALID_PARAMETER);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.8", 95, 1, &grade_test) == GRADE_INVALID_PARAMETER);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 105, 1, &grade_test) == GRADE_INVALID_PARAMETER);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", -2, 1, &grade_test) == GRADE_INVALID_PARAMETER);

    grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, -1, &grade_test) == GRADE_INVALID_PARAMETER);

    gradeDestroy(grade_test);
    return true;
}

static bool testGradeCopy() {
    //Grade gradeCopy(Grade grade);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(getGradeNumber(grade_test) == 95);
    ASSERT_TEST(getCoursePointsX2(grade_test) == 7);
    ASSERT_TEST(getCourseId(grade_test) == 11071);
    ASSERT_TEST(getSemester(grade_test) == 1);

    Grade grade_test_copy = NULL;
    grade_test_copy = gradeCopy(grade_test);
    ASSERT_TEST(grade_test_copy != NULL);
    ASSERT_TEST(getGradeNumber(grade_test) == 95);
    ASSERT_TEST(getCoursePointsX2(grade_test) == 7);
    ASSERT_TEST(getCourseId(grade_test) == 11071);
    ASSERT_TEST(getSemester(grade_test) == 1);

    grade_test = NULL;
    grade_test_copy = gradeCopy(grade_test);
    ASSERT_TEST(grade_test_copy == NULL);

    gradeDestroy(grade_test);
    return true;
}

static bool testGradeCompare() {
    //int gradeCompare(Grade grade1, Grade grade2);
    Grade grade_test1 = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test1) == GRADE_OK);
    Grade grade_test2 = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test2) == GRADE_OK);
    Grade grade_test3 = NULL;
    ASSERT_TEST(gradeCreate(11071, "4", 95, 1, &grade_test3) == GRADE_OK);

    int compare_result = gradeCompare(grade_test1, grade_test2);
    ASSERT_TEST(compare_result == 0);
    compare_result = gradeCompare(grade_test2, grade_test1);
    ASSERT_TEST(compare_result == 0);
    compare_result = gradeCompare(grade_test1, grade_test3);
    ASSERT_TEST(compare_result == 0);
    compare_result = gradeCompare(grade_test3, grade_test2);
    ASSERT_TEST(compare_result == 0);

    ASSERT_TEST(gradeCreate(11071, "3.5", 100, 1, &grade_test3) == GRADE_OK);
    compare_result = gradeCompare(grade_test3, grade_test1);
    ASSERT_TEST(compare_result == 1);
    compare_result = gradeCompare(grade_test1, grade_test3);
    ASSERT_TEST(compare_result == -1);

    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 2, &grade_test3) == GRADE_OK);
    compare_result = gradeCompare(grade_test3, grade_test1);
    ASSERT_TEST(compare_result == -1);
    compare_result = gradeCompare(grade_test1, grade_test3);
    ASSERT_TEST(compare_result == 1);

    ASSERT_TEST(gradeCreate(11072, "3.5", 95, 1, &grade_test3) == GRADE_OK);
    compare_result = gradeCompare(grade_test3, grade_test1);
    ASSERT_TEST(compare_result == -1);
    compare_result = gradeCompare(grade_test1, grade_test3);
    ASSERT_TEST(compare_result == 1);

    ASSERT_TEST(gradeCreate(11072, "3.5", 100, 2, &grade_test3) == GRADE_OK);
    compare_result = gradeCompare(grade_test3, grade_test1);
    ASSERT_TEST(compare_result == 1);
    compare_result = gradeCompare(grade_test1, grade_test3);
    ASSERT_TEST(compare_result == -1);

    grade_test3 = NULL;
    compare_result = gradeCompare(grade_test3, grade_test1);
    ASSERT_TEST(compare_result == -1);
    compare_result = gradeCompare(grade_test1, grade_test3);
    ASSERT_TEST(compare_result == 1);

    gradeDestroy(grade_test1);
    gradeDestroy(grade_test2);
    gradeDestroy(grade_test3);
    return true;
}

static bool testIsGradeIsForCourse() {
    //bool isGradeIsForCourse(ListElement grade, ListFilterKey course_id);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);

    int course_id1 = 11071, course_id2 = 11072;
    ASSERT_TEST(isGradeIsForCourse((ListElement)grade_test, (ListFilterKey)&course_id1) == true);
    ASSERT_TEST(isGradeIsForCourse((ListElement)grade_test, (ListFilterKey)&course_id2) == false);
    ASSERT_TEST(isGradeIsForCourse((ListElement)NULL, (ListFilterKey)&course_id1) == false);
    ASSERT_TEST(isGradeIsForCourse((ListElement)grade_test, (ListFilterKey)NULL) == false);

    gradeDestroy(grade_test);
    return true;
}

static bool testGetGradeNumber() {
    //int getGradeNumber(Grade grade);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(getGradeNumber(grade_test) == 95);
    ASSERT_TEST(getGradeNumber(NULL) == -1);

    gradeDestroy(grade_test);
    return true;
}

static bool testGetCoursePointsX2() {
    //int getCoursePointsX2(Grade grade);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(getCoursePointsX2(grade_test) == 7);
    ASSERT_TEST(getCoursePointsX2(NULL) == -1);

    gradeDestroy(grade_test);
    return true;
}

static bool testGetCourseId() {
    //int getCourseId(Grade grade);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(getCourseId(grade_test) == 11071);
    ASSERT_TEST(getCourseId(NULL) == -1);

    gradeDestroy(grade_test);
    return true;
}

static bool testGetSemester() {
    //int getSemester(Grade grade);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(getSemester(grade_test) == 1);
    ASSERT_TEST(getSemester(NULL) == -1);

    gradeDestroy(grade_test);
    return true;
}

static bool testGradeUpdateGradeNumber() {
    //GradeResult gradeUpdateGradeNumber(Grade grade, int new_grade);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);
    ASSERT_TEST(gradeUpdateGradeNumber(grade_test, 90) == GRADE_OK);
    ASSERT_TEST(gradeUpdateGradeNumber(grade_test, 105) == GRADE_INVALID_PARAMETER);
    ASSERT_TEST(gradeUpdateGradeNumber(grade_test, -2) == GRADE_INVALID_PARAMETER);
    ASSERT_TEST(gradeUpdateGradeNumber(NULL, 90) == GRADE_NULL_ARGUMENT);

    gradeDestroy(grade_test);
    return true;
}

static bool testGradePrintInfo() {
    //void gradePrintInfo(Grade grade, FILE* output_channel);
    Grade grade_test = NULL;
    ASSERT_TEST(gradeCreate(11071, "3.5", 95, 1, &grade_test) == GRADE_OK);
    printf("\n");
    gradePrintInfo(grade_test, stdout);
    // should print
    // Course: 011071            Points: 3.5             Grade: 95

    gradePrintInfo(NULL, stdout);

    gradePrintInfo(grade_test, NULL);

    gradeDestroy(grade_test);
    return true;
}


int runGradeTests() {
    RUN_TEST(testGradeCreate);
    RUN_TEST(testGradeCopy);
    RUN_TEST(testGradeCompare);
    RUN_TEST(testIsGradeIsForCourse);
    RUN_TEST(testGetGradeNumber);
    RUN_TEST(testGetCoursePointsX2);
    RUN_TEST(testGetCourseId);
    RUN_TEST(testGetSemester);
    RUN_TEST(testGradeUpdateGradeNumber);
    RUN_TEST(testGradePrintInfo);

    int final;
    scanf("%d", &final);
    return 0;
}