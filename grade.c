#include "grade.h"
#include <string.h>
#include <stdlib.h>
#include "assert.h"

typedef struct grade_t {
    int course_id;
    int course_points_integer;
    int course_points_partial;
    int grade_number;
};

/**
 * parsePoints - inner function. parses the char* points parameter into 2 ints -
 * the number part (before the '.'), and the partial part (after the '.') - if exists.
 * @param points - the points parameter
 * @param number - pointer to the result of the number part
 * @param partial - pointer to the result of the partial part
 * @return
 * false - if the points parameter is not in valid format
 * true - otherwise (the parse succeed)
 */
static bool parsePoints(char* points, int* number, int* partial) {
    *partial = 0; // if there is no '.' the partial part is 0
    int number_part_length = (int)strlen(points);
    // check the partial part (if exists)
    if (number_part_length > 1 && points[strlen(points) - 1] == '.') {
        if (points[strlen(points)] != '0' && points[strlen(points)] != '5') return false;
        *partial = points[strlen(points)] - '0';
        number_part_length -= 2;
    }
    // X part must not be empty
    if (number_part_length == 0) return false;
    // check the number part
    *number = 0;
    for (int i=0; i < number_part_length; i++) {
        if (points[i] < '0' || points[i] > '9') return false;
        *number = *number * 10 + points[i] - '0';
    }
    return true;
}

/**
 * gradeCreate - creates new grade
 * @param course_id - the id of the course the grade is attached to. must be positive number lower than 1000000000.
 * @param points - the number of points that the course worth. must be in the format of:
 * X or X.0 or X.5
 * while X is not empty and contains only digits
 * @param grade_number - the grade points to add. must be integer betweeen 0 and 100.
 * @param grade - pointer to the grade that will be created
 * @return
 * GRADE_NULL_ARGUMENT - if points is NULL
 * GRADE_OUT_OF_MEMORY - if there was a memory error
 * GRADE_INVALID_PARAMETER - if one of the parameters in not valid (course_id is negative or higher than 1000000000, 
 * points is not in the valid format, or grade_number is not integer between 0 and 100)
 * GRADE_OK - otherwise
 */
GradeResult gradeCreate(int course_id, char* points, int grade_number, Grade *grade) {
    if (points == NULL) return GRADE_NULL_ARGUMENT;
    if (course_id >= 1000000000 || course_id < 0) return GRADE_INVALID_PARAMETER;
    int course_points_integer, course_points_partial;
    if (parsePoints(points, &course_points_integer, &course_points_partial) == false) return GRADE_INVALID_PARAMETER;
    if (grade_number < 0 || grade_number > 100) return GRADE_INVALID_PARAMETER;

    Grade new_grade = (Grade) malloc(sizeof(*new_grade));
    if (new_grade == NULL) return GRADE_OUT_OF_MEMORY;
    new_grade->course_id = course_id;
    new_grade->course_points_integer = course_points_integer;
    new_grade->course_points_partial = course_points_partial;
    new_grade->grade_number = grade_number;
    *grade = new_grade;
    return GRADE_OK;
}

/**
 * isGradeIsForCourse - checks if given grade is grade of course with the given id
 * @param grade - the grade to check
 * @param course_id - the id of the course to check
 * @return
 * true if the grade is grade of the given course and false otherwise
 */
bool isGradeIsForCourse(ListElement grade, ListFilterKey course_id) {
    return ((Grade)grade)->course_id == *(int*)course_id;
}

/**
 * gradeCopy: Creates a copy of target grade.
 *
 * @param grade - Target grade.
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A Grade with the same data as given grade otherwise.
 */
Grade gradeCopy(Grade grade) {
    if (grade == NULL) return NULL;

    Grade new_grade = (Grade) malloc(sizeof(*new_grade));
    if (new_grade == NULL) return NULL;
    new_grade->course_id = grade->course_id;
    new_grade->course_points_integer = grade->course_points_integer;
    new_grade->course_points_partial = grade->course_points_partial;
    new_grade->grade_number = grade->grade_number;
    return new_grade;
}

/**
 * gradeDestroy - deallocate all the data of the grade
 * @param grade - the grade to destroy
 * if grade is NULL nothing will be done
 */
void gradeDestroy(Grade grade) {
    free(grade);
}