#include "test_utilities.h"
#include "../semester.h"
#include "stdio.h"

static bool testSemesterCreate() {
    //SemesterResult semesterCreate(int semester_number, Semester *semester);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterCreate(-2, &semester_test) == SEMESTER_INVALID_PARAMETER);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterAddGrade() {
    //SemesterResult semesterAddGrade(Semester semester, int course_id, char* points, int grade);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(NULL, 11071, "3.5", 95) == SEMESTER_OUT_OF_MEMORY);
    ASSERT_TEST(semesterAddGrade(semester_test, -1, "3.5", 95) == SEMESTER_INVALID_PARAMETER);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.8", 95) == SEMESTER_INVALID_PARAMETER);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3a.5", 95) == SEMESTER_INVALID_PARAMETER);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 105) == SEMESTER_INVALID_PARAMETER);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", -2) == SEMESTER_INVALID_PARAMETER);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterCopy() {
    //Semester semesterCopy(Semester semester);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    Semester semester_test2 = semesterCopy(NULL);
    ASSERT_TEST(semester_test2 == NULL);
    semester_test2 = semesterCopy(semester_test);
    ASSERT_TEST(semesterCompare(semester_test, semester_test2) == 0);

    semesterDestroy(semester_test);
    semesterDestroy(semester_test2);
    return true;
}

static bool testSemesterCompare() {
    //int semesterCompare(Semester semester1, Semester semester2);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);

    Semester semester_test2 = NULL;
    ASSERT_TEST(semesterCreate(2, &semester_test2) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test2, 11070, "1.5", 70) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test2, 11070, "2", 60) == SEMESTER_OK);

    Semester semester_test3 = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test3) == SEMESTER_OK);

    ASSERT_TEST(semesterCompare(semester_test, semester_test2) == -1);
    ASSERT_TEST(semesterCompare(semester_test2, semester_test) == 1);
    ASSERT_TEST(semesterCompare(semester_test, semester_test3) == 0);
    ASSERT_TEST(semesterCompare(semester_test3, semester_test) == 0);

    semesterDestroy(semester_test);
    semesterDestroy(semester_test2);
    semesterDestroy(semester_test3);
    return true;
}

static bool testGetSemesterFromSet() {
    //Semester getSemesterFromSet(Set set, int semester);
    Set test_set = setCreate((SetElement)semesterCopy, (SetElement)semesterDestroy, (SetElement)semesterCompare);
    ASSERT_TEST(test_set != NULL);

    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);

    Semester semester_test2 = NULL;
    ASSERT_TEST(semesterCreate(2, &semester_test2) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test2, 11070, "1.5", 70) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test2, 11070, "2", 60) == SEMESTER_OK);

    ASSERT_TEST(setAdd(test_set, semester_test) == SET_SUCCESS);
    ASSERT_TEST(setAdd(test_set, semester_test2) == SET_SUCCESS);

    Semester set_semester1 = getSemesterFromSet(test_set, 1);
    ASSERT_TEST(semesterCompare(set_semester1, semester_test) == 0);
    Semester set_semester2 = getSemesterFromSet(test_set, 2);
    ASSERT_TEST(semesterCompare(set_semester2, semester_test2) == 0);
    Semester set_semester3 = getSemesterFromSet(test_set, 3);
    ASSERT_TEST(set_semester3 == NULL);

    semesterDestroy(semester_test);
    semesterDestroy(semester_test2);
    //semesterDestroy(set_semester1);
    //semesterDestroy(set_semester2);
    setDestroy(test_set);
    return true;
}

static bool testSemesterRemoveGrade() {
    //SemesterResult semesterRemoveGrade(Semester semester, int course_id);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);

    ASSERT_TEST(semesterRemoveGrade(semester_test, 11071) == SEMESTER_OK);
    ASSERT_TEST(semesterRemoveGrade(semester_test, 11071) == SEMESTER_COURSE_DOES_NOT_EXIST);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterGetCourseLastGradeObject() {
    //Grade semesterGetCourseLastGradeObject(Semester semester, int course_id);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 70) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);

    Grade grade_result = semesterGetCourseLastGradeObject(semester_test, 11071);
    ASSERT_TEST(getGradeNumber(grade_result) == 95 );
    ASSERT_TEST(getCourseId(grade_result) == 11071 );
    ASSERT_TEST(getCoursePointsX2(grade_result) == 7 );
    ASSERT_TEST(semesterGetCourseLastGradeObject(semester_test, 234118) == NULL);

    //gradeDestroy(grade_result);
    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterGetCourseLastGrade() {
    //int semesterGetCourseLastGrade(Semester semester, int course_id);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 70) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);

    ASSERT_TEST(semesterGetCourseLastGrade(semester_test, 11071) == 95 );
    ASSERT_TEST(semesterGetCourseLastGrade(semester_test, 234118) == -1);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterGetCourseBestGrade() {
    //int semesterGetCourseBestGrade(Semester semester, int course_id);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 80) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 60) == SEMESTER_OK);

    ASSERT_TEST(semesterGetCourseBestGrade(semester_test, 11071) == 80 );
    ASSERT_TEST(semesterGetCourseBestGrade(semester_test, 234118) == -1);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterGetCoursePointsX2() {
    //int semesterGetCoursePointsX2(Semester semester, int course_id);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 80) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 60) == SEMESTER_OK);

    ASSERT_TEST(semesterGetCoursePointsX2(semester_test, 11071) == 7 );
    ASSERT_TEST(semesterGetCoursePointsX2(semester_test, 234118) == -1);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterUpdateGrade() {
    //SemesterResult semesterUpdateGrade(Semester semester, int course_id, int new_grade);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 70) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);

    ASSERT_TEST(semesterGetCourseLastGrade(semester_test, 11071) == 95 );
    ASSERT_TEST(semesterUpdateGrade(semester_test, 11071, 80) == SEMESTER_OK);
    ASSERT_TEST(semesterGetCourseLastGrade(semester_test, 11071) == 80);
    ASSERT_TEST(semesterUpdateGrade(semester_test, 234118, 80) == SEMESTER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(semesterUpdateGrade(semester_test, 11071, 200) == SEMESTER_INVALID_PARAMETER);
    ASSERT_TEST(semesterUpdateGrade(semester_test, 11071, -2) == SEMESTER_INVALID_PARAMETER);
    ASSERT_TEST(semesterUpdateGrade(semester_test, 234118, 200) == SEMESTER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(semesterUpdateGrade(NULL, 11071, 80) == SEMESTER_OUT_OF_MEMORY);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterPrintAllSemesterGrades() {
    //void semesterPrintAllSemesterGrades(Semester semester, FILE* output_channel);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 70) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);
    printf("\n");
    semesterPrintAllSemesterGrades(semester_test, stdout);
    // should print
    // Course: 011071            Points: 3.5             Grade: 70
    // Course: 011072            Points: 4.0             Grade: 90
    // Course: 011071            Points: 3.5             Grade: 95
    semesterPrintAllSemesterGrades(NULL, stdout);
    semesterPrintAllSemesterGrades(semester_test, NULL);
    semesterPrintAllSemesterGrades(NULL, NULL);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterGetCoursesSet() {
    //SemesterResult semesterGetCoursesSet(Semester semester, Set *set);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 70) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11072, "4", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 11071, "3.5", 95) == SEMESTER_OK);

    Set course_set = NULL;
    ASSERT_TEST(semesterGetCoursesSet(semester_test, &course_set) == SEMESTER_OK);
    int counter = 0;
    SET_FOREACH(int*, current_course, course_set) {
        if (counter == 0) {
            ASSERT_TEST(*current_course == 11071);
            counter ++;
        } else if (counter == 1) {
            ASSERT_TEST(*current_course == 11072);
            counter++;
        }
    }
    ASSERT_TEST(counter == 2);

    ASSERT_TEST(semesterGetCoursesSet(NULL, &course_set) == SEMESTER_OUT_OF_MEMORY);

    semesterDestroy(semester_test);
    setDestroy(course_set);
    return true;
}

static bool testSemesterGetTotalCoursePointsX2() {
    //int semesterGetTotalCoursePointsX2(Semester semester);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 51) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 114071, "3.5", 75) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterGetTotalCoursePointsX2(semester_test) == 19);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterGetFailedCoursePointsX2() {
    //int semesterGetFailedCoursePointsX2(Semester semester);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 51) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 114071, "3.5", 75) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterGetFailedCoursePointsX2(semester_test) == 6);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterGetEffectiveCoursePointsX2() {
    //int semesterGetEffectiveCoursePointsX2(Semester semester);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 51) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 114071, "3.5", 75) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterGetEffectiveCoursePointsX2(semester_test) == 13);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterGetEffectiveGradeSumX2() {
    //int semesterGetEffectiveGradeSumX2(Semester semester);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 51) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 114071, "3.5", 75) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 90) == SEMESTER_OK);
    ASSERT_TEST(semesterGetEffectiveGradeSumX2(semester_test) == 1065);

    semesterDestroy(semester_test);
    return true;
}

static bool testSemesterPrintInfo() {
    //SemesterResult semesterPrintInfo(Semester semester, FILE* output_channel);
    Semester semester_test = NULL;
    ASSERT_TEST(semesterCreate(1, &semester_test) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 51) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 114071, "3.5", 75) == SEMESTER_OK);
    ASSERT_TEST(semesterAddGrade(semester_test, 234122, "3", 90) == SEMESTER_OK);
    printf("\n");
    semesterPrintInfo(semester_test, stdout);
    //should print
    //========================================
    //Semester 1		Points: 6.5		Average:81.92 	Success: 68%

    semesterDestroy(semester_test);
    return true;
}


int runAllSemesterTests() {
    RUN_TEST(testSemesterCreate);
    RUN_TEST(testSemesterAddGrade);
    RUN_TEST(testSemesterCopy);
    RUN_TEST(testSemesterCompare);
    RUN_TEST(testGetSemesterFromSet);
    RUN_TEST(testSemesterRemoveGrade);
    RUN_TEST(testSemesterGetCourseLastGradeObject);
    RUN_TEST(testSemesterGetCourseLastGrade);
    RUN_TEST(testSemesterGetCourseBestGrade);
    RUN_TEST(testSemesterUpdateGrade);
    RUN_TEST(testSemesterGetCoursePointsX2);
    RUN_TEST(testSemesterPrintAllSemesterGrades);
    RUN_TEST(testSemesterGetCoursesSet);
    RUN_TEST(testSemesterGetTotalCoursePointsX2);
    RUN_TEST(testSemesterGetFailedCoursePointsX2);
    RUN_TEST(testSemesterGetEffectiveCoursePointsX2);
    RUN_TEST(testSemesterGetEffectiveGradeSumX2);
    RUN_TEST(testSemesterPrintInfo);


    int final;
    scanf("%d", &final);
    return 0;
}