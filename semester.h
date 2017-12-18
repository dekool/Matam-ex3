#ifndef EX3_SEMESTER_H
#define EX3_SEMESTER_H

#include "set.h"
#include "list.h"

typedef struct semester_t *Semester;

/** Type used for returning error codes from functions */
typedef enum SemesterResult_t {
    SEMESTER_OK,
    SEMESTER_NULL_ARGUMENT,
    SEMESTER_OUT_OF_MEMORY,
    SEMESTER_INVALID_PARAMETER,
    SEMESTER_COURSE_DOES_NOT_EXIST
} SemesterResult;

/**
* semesterCreate - creates a new semester grade
* @param semester - the semester number. must be positive number
* @return
* SEMESTER_OUT_OF_MEMORY - if there was a memory error
* SEMESTER_INVALID_PARAMETER - if semester is not valid (negative number)
* SEMESTER_OK - otherwise
*/
SemesterResult semesterCreate(int semester_number, Semester *semester);

/**
 * semesterCopy - creates a copy of target semester
 * @param semester - target semester
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A Semester with the same data as given semester otherwise.
 */
Semester semesterCopy(Semester semester);

/**
 * semesterCompare - compare between 2 semesters by the semester number
 * @param semester1 - first semester to compare
 * @param semester2  - second semester to compare
 * @return
 * 1 - if semester1's semester number is bigger
 * -1 - if semester2's semester number is bigger
 * 0 - if both semesters has the same semester number
 */
int semesterCompare(Semester semester1, Semester semester2);

/**
 * getSemesterFromSet - return the semester pointer of the semester with given semester from given set.
 * @param set - the set to search the semester in
 * @param semester - the semester of the semester to search
 * @return
 * pointer to the semester with the given semester (if found).
 * if the semester does not found, returns NULL
 */
Semester getSemesterFromSet(Set set, int semester);

/**
 * semesterAddGrade - adds the given grade as last grade of the given semester
 * @param semester - the semester to add the grade to
 * @param course_id - the id of the course is grade is attached to (must be positive number lower than 1000000000)
 * @param points - the number of points that the course worth. must be in the format of:
 * X or X.0 or X.5
 * while X is not empty and contains only digits
 * @param grade - the grade to add (must be number between 0 and 100)
 * @return
 * SEMESTER_OUT_OF_MEMORY - if theer was a memory error
 * SEMESTER_INVALID_PARAMETER - if the one of the parameters is not valid
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterAddGrade(Semester semester, int course_id, char* points, int grade);

/**
 * semesterRemoveGrade - removes the last grade of the course with the given course id
 * @param semester - the semester to remove the grade from
 * @param course_id - the id of the course to remove the grade from
 * @return 
 * SEMESTER_OUT_OF_MEMORY - if there was a memory error
 * SEMESTER_COURSE_DOES_NOT_EXIST -if there are no grades for course with the given id in this semester
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterRemoveGrade(Semester semester, int course_id);

/**
 * semesterDestroy - deallocate all the data of the semester
 * @param semester - the semester to destroy
 * if semester is NULL nothing will be done
 */
void semesterDestroy(Semester semester);

#endif //EX3_SEMESTER_H
