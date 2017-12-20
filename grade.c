#include "grade.h"
#include <string.h>
#include <stdlib.h>
#include "assert.h"

typedef struct grade_t {
    int course_id;
    int course_points_integer;
    int course_points_partial;
    int grade_number;
    int semester_number;
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
 * @param grade_number - the grade points to add. must be integer between 0 and 100.
 * @param semester_number - the semester the grade is attached to. must be positive number.
 * @param grade - pointer to the grade that will be created
 * @return
 * GRADE_NULL_ARGUMENT - if points is NULL
 * GRADE_OUT_OF_MEMORY - if there was a memory error
 * GRADE_INVALID_PARAMETER - if one of the parameters in not valid (course_id is negative or higher than 1000000000, 
 * points is not in the valid format, or grade_number is not integer between 0 and 100)
 * GRADE_OK - otherwise
 */
GradeResult gradeCreate(int course_id, char* points, int grade_number, int semester_number, Grade *grade) {
    if (points == NULL) return GRADE_NULL_ARGUMENT;
    if (course_id >= 1000000000 || course_id < 0) return GRADE_INVALID_PARAMETER;
    int course_points_integer, course_points_partial;
    if (parsePoints(points, &course_points_integer, &course_points_partial) == false) return GRADE_INVALID_PARAMETER;
    if (grade_number < 0 || grade_number > 100) return GRADE_INVALID_PARAMETER;
    if (semester_number < 0) return GRADE_INVALID_PARAMETER;

    Grade new_grade = (Grade) malloc(sizeof(*new_grade));
    if (new_grade == NULL) return GRADE_OUT_OF_MEMORY;
    new_grade->course_id = course_id;
    new_grade->course_points_integer = course_points_integer;
    new_grade->course_points_partial = course_points_partial;
    new_grade->grade_number = grade_number;
    new_grade->semester_number = semester_number;
    *grade = new_grade;
    return GRADE_OK;
}

/**
 * gradeCompare - compares between 2 grades.
 * grade1 is bigger than grade2 if it's grade number is higher. if both grade numbers are equal, the grade with the
 * smaller semester number is higher. if the semester numbers are also equal, the grade with the smaller course id is
 * higher. if the course ids are also equal - the grade are consider equal.
 * @param grade1 - first grade to check
 * @param grade2 - second grade to check
 * @return
 * 1 - if first grade is higher
 * 0 - if grades are equal
 * -1 - if second grade is higher
 */
int gradeCompare(Grade grade1, Grade grade2) {
    if (grade2 == NULL) return 1;
    if (grade1 == NULL) return -1;
    if (grade1->grade_number > grade2->grade_number) {
        return 1;
    } else if (grade1->grade_number == grade2->grade_number) {
        if (grade1->semester_number < grade2->semester_number) {
            return 1;
        } else if (grade1->semester_number == grade2->semester_number) {
            if (grade1->course_id < grade2->course_id) {
                return 1;
            } else if (grade1->course_id == grade2->course_id) {
                return 0;
            }
        }
    }
    return -1;
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
 * getGradeNumber - return the grade in given Grade object
 * @param grade - the grade to get the grade from
 * @return the grade number. if the given Grade is NULL returns -1
 */
int getGradeNumber(Grade grade) {
    if (grade == NULL) return -1;
    return grade->grade_number;
}

/**
 * getCourseId - return the course id the given grade is attached to
 * @param grade - the grade to get the course id from
 * @return the course id. if the given Grade is NULL returns -1
 */
int getCourseId(Grade grade) {
    if (grade == NULL) return -1;
    return grade->course_id;
}

/**
 * getSemester - return the semester that the given grade is attached to
 * @param grade - the grade to get the semester from
 * @return the semester number. if the given Grade is NULL returns -1
 */
int getSemester(Grade grade) {
    if (grade == NULL) return -1;
    return grade->semester_number;
}

/**
 * getCoursePointsX2 - return the course points multiple by 2, of the course the grade is attached to
 * @param grade - the grade to get the course points from
 * @return the course points multiple by 2. if the given Grade is NULL return -1
 */
int getCoursePointsX2(Grade grade) {
    if (grade == NULL) return -1;
    int points_x2 = (grade->course_points_integer)*2;
    if (grade->course_points_partial != 0) {
        points_x2 += 1; // if the partial part is not 0, it must be 0.5
    }
    return points_x2;
}

/**
 * gradeUpdateGradeNumber - updates the grade number in given grade to new grade.
 * @param grade - the grade to update it's grade number
 * @param new_grade - the new grade number (must be integer between 0 and 100)
 * @return
 * GRADE_INVALID_PARAMETER - if the new grade is not valid
 * GRADE_NULL_ARGUMENT - if the grade given is NULL
 * GRADE_OK - otherwise
 */
GradeResult gradeUpdateGradeNumber(Grade grade, int new_grade) {
    if (new_grade < 0 || new_grade > 100) return GRADE_INVALID_PARAMETER;
    if (grade == NULL) return GRADE_NULL_ARGUMENT;
    grade->grade_number = new_grade;
    return GRADE_OK;
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
    new_grade->semester_number = grade->semester_number;
    return new_grade;
}

/**
 * gradePrintInfo - print the info of the given grade into the given output channel
 * @param grade - the grade to print it's info
 * @param output_channel - the channel to print the info to
 */
void gradePrintInfo(Grade grade, FILE* output_channel) {
    assert(grade != NULL);
    mtmPrintGradeInfo(output_channel, grade->course_id, getCoursePointsX2(grade), grade->grade_number);
}

/**
 * gradeDestroy - deallocate all the data of the grade
 * @param grade - the grade to destroy
 * if grade is NULL nothing will be done
 */
void gradeDestroy(Grade grade) {
    free(grade);
}