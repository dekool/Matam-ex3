#ifndef EX3_GRADE_H
#define EX3_GRADE_H

#include "list.h"
#include "set.h"

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
 * @param grade_number - the grade points to add. must be integer betweeen 0 and 100.
 * @param grade - pointer to the grade that will be created
 * @return
 * GRADE_NULL_ARGUMENT - if points is NULL
 * GRADE_OUT_OF_MEMORY - if there was a memory error
 * GRADE_INVALID_PARAMETER - if one of the parameters in not valid (course_id is negative or higher than 1000000000, 
 * points is not in the valid format, or grade_number is not integer between 0 and 100)
 * GRADE_OK - otherwise
 */
GradeResult gradeCreate(int course_id, char* points, int grade_number, Grade *grade);

/**
 * gradeCopy: Creates a copy of target grade.
 *
 * @param grade - Target grade.
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A Grade with the same data as given grade otherwise.
 */
Grade gradeCopy(Grade grade);

/**
 * isGradeIsForCourse - checks if given grade is grade of course with the given id
 * @param grade - the grade to check
 * @param course_id - the id of the course to check
 * @return
 * true if the grade is grade of the given course and false otherwise
 */
bool isGradeIsForCourse(ListElement grade, ListFilterKey course_id);

/**
 * gradeDestroy - deallocate all the data of the grade
 * @param grade - the grade to destroy
 * if grade is NULL nothing will be done
 */
void gradeDestroy(Grade grade);

#endif //EX3_GRADE_H
