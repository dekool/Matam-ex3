#ifndef EX3_GRADE_H
#define EX3_GRADE_H

#include "list.h"
#include "set.h"
#include "mtm_ex3.h"

typedef struct grade_t *Grade;

/** Type used for returning error codes from functions */
typedef enum GradeResult_t {
    GRADE_OK,
    GRADE_NULL_ARGUMENT,
    GRADE_OUT_OF_MEMORY,
    GRADE_INVALID_PARAMETER
} GradeResult;

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
GradeResult gradeCreate(int course_id, char* points, int grade_number,int semester_number, Grade *grade);

/**
 * gradeCopy: Creates a copy of target grade.
 *
 * @param grade - Target grade.
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A Grade with the same data as given grade otherwise.
 */
ListElement gradeCopy(ListElement grade);

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
int gradeCompare(Grade grade1, Grade grade2);

/**
 * isGradeIsForCourse - checks if given grade is grade of course with the given id
 * @param grade - the grade to check
 * @param course_id - the id of the course to check
 * @return
 * true if the grade is grade of the given course and false otherwise
 */
bool isGradeIsForCourse(Grade grade, int course_id);

/**
 * getGradeNumber - return the grade in given Grade object
 * @param grade - the grade to get the grade from
 * @return the grade number. if the given Grade is NULL returns -1
 */
int getGradeNumber(Grade grade);

/**
 * getCourseId - return the course id that the given grade is attached to
 * @param grade - the grade to get the course id from
 * @return the course id. if the given Grade is NULL returns -1
 */
int getCourseId(Grade grade);

/**
 * getSemester - return the semester that the given grade is attached to
 * @param grade - the grade to get the semester from
 * @return the semester number. if the given Grade is NULL returns -1
 */
int getSemester(Grade grade);

/**
 * getCoursePointsX2 - return the course points multiple by 2, of the course the grade is attached to
 * @param grade - the grade to get the course points from
 * @return the course points multiple by 2. if the given Grade is NULL return -1
 */
int getCoursePointsX2(Grade grade);

/**
 * gradeUpdateGradeNumber - updates the grade number in given grade to new grade.
 * @param grade - the grade to update it's grade number
 * @param new_grade - the new grade number (must be integer between 0 and 100)
 * @return
 * GRADE_NULL_ARGUMENT - if the grade given is NULL
 * GRADE_INVALID_PARAMETER - if the new grade is not valid
 * GRADE_OK - otherwise
 */
GradeResult gradeUpdateGradeNumber(Grade grade, int new_grade);

/**
 * gradePrintInfo - print the info of the given grade into the given output channel
 * @param grade - the grade to print it's info
 * @param output_channel - the channel to print the info to
 */
void gradePrintInfo(Grade grade, FILE* output_channel);

/**
 * gradeDestroy - deallocate all the data of the grade
 * @param grade - the grade to destroy
 * if grade is NULL nothing will be done
 */
void gradeDestroy(ListElement grade);

#endif //EX3_GRADE_H
