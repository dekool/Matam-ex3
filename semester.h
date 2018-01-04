#ifndef EX3_SEMESTER_H
#define EX3_SEMESTER_H

#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include "grade.h"

typedef struct semester_t *Semester;

/** Type used for returning error codes from functions */
typedef enum SemesterResult_t {
    SEMESTER_OK,
    SEMESTER_GOT_EMPTY,
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
SetElement semesterCopy(SetElement semester);

/**
 * semesterCompare - compare between 2 semesters by the semester number
 * @param semester1 - first semester to compare
 * @param semester2  - second semester to compare
 * @return
 * 1 - if semester1's semester number is bigger
 * -1 - if semester2's semester number is bigger
 * 0 - if both semesters has the same semester number
 */
int semesterCompare(SetElement semester1, SetElement semester2);

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
 * @param course_id - the id of the course is grade is attached to (must be positive number lower than 1000000)
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
 * semesterGetCourseLastGradeObject - get the last grade object inserted for course with the given course_id for the given
 * semester.
 * if there are no grades for this course in this semester NULL is returned
 * @param semester - the semester to search the course grade in
 * @param course_id - the id of the course to search
 * @return
 * the last grade object inserted for the given course in the given semester, or NULL if such grade do not exist
 */
Grade semesterGetCourseLastGradeObject(Semester semester, int course_id);

/**
 * semesterGetCourseLastGrade - get the last grade inserted for course with the given course_id for the given semester.
 * if there are no grade for this course in this semester -1 is returned
 * @param semester - the semester to search the course grade in
 * @param course_id - the id of the course to search
 * @return
 * the last grade inserted for the given course in the given semester, or -1 if such grade do not exist
 */
int semesterGetCourseLastGrade(Semester semester, int course_id);

/**
 * semesterGetCourseBestGrade - get the best grade inserted for course with the given course_id for the given
 * semester.
 * if there are no grades for this course in this semester -1 is returned
 * @param semester - the semester to search the course grade in
 * @param course_id - the id of the course to search
 * @return
 * the best grade inserted for the given course in the given semester, or -1 if such grade do not exist
 */
int semesterGetCourseBestGrade(Semester semester, int course_id);

/**
 * semesterGetCoursePointsX2 - get the points of the course with the given course_id multiply by 2.
 * if there are no grades for this course in this semester -1 is returned
 * @param semester - the semester to search the course points in
 * @param course_id - the id of the course to search
 * @return
 * the points of the given course in the given semester, or -1 if such grade do not exist
 */
int semesterGetCoursePointsX2(Semester semester, int course_id);

/**
 * semesterUpdateGrade - replace the last grade of the course with the given id in the given semester with the given
 * grade.
 * @param semester - the semester to update the grade in
 * @param course_id - the id of the course to update it's grade
 * @param new_grade - the new grade (must be integer between 0 and 100)
 * @return
 * SEMESTER_INVALID_PARAMETER - if the new grade is not valid
 * SEMESTER_COURSE_DOES_NOT_EXIST - if there are no grade for the given course in this semester
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterUpdateGrade(Semester semester, int course_id, int new_grade);

/**
 * semesterPrintAllSemesterGrades - prints all the grades for all courses in the given semester,
 * into the given output channel. the grades are printed in the order they were inserted
 * @param semester - the semester to print it's grades
 * @param output_channel - the channel to print the grades to
 */
void semesterPrintAllSemesterGrades(Semester semester, FILE* output_channel);

/**
 * semesterGetCoursesSet - get a set of ids of all the courses in the given semester.
 * if there are no courses in the given semester or there was a memory error, NULL is returned
 * @param semester - the semester to search the courses in
 * @param set - pointer to the set that will be returned
 * @return
 * SEMESTER_OUT_OF_MEMORY - if there was a memory error
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterGetCoursesSet(Semester semester, Set *set);

/**
 * semesterGetTotalCoursePointsX2 - return the total sum of course points multiple by 2 of the courses in the semester.
 * if the semester is NULL return -1, and if there are no courses in the semester return 0
 * @param semester - the semester to sum the points in
 * @return the total sum of course points multiple by 2
 */
int semesterGetTotalCoursePointsX2(Semester semester);

/**
 * semesterGetFailedCoursePointsX2 - return the total sum of course points multiple by 2 of the failed courses in the
 * semester.
 * if the semester is NULL return -1, and if there are no courses in the semester return 0
 * @param semester - the semester to sum the points in
 * @return the total sum of failed course points multiple by 2
 */
int semesterGetFailedCoursePointsX2(Semester semester);

/**
 * semesterGetEffectiveCoursePointsX2 - return the total sum of effective course points multiple by 2 of the courses
 * in the semester.
 * if the semester is NULL or there was memory error return -1, and if there are no courses in the semester return 0
 * @param semester - the semester to sum the points in
 * @return the total sum of effective course points multiple by 2
 */
int semesterGetEffectiveCoursePointsX2(Semester semester);

/**
 * semesterGetEffectiveGradeSumX2 - return the total sum of grades of the effective courses in the semester, multiply
 * by the course points and multiple by 2
 * if the semester is NULL or there was memory error return -1, and if there are no courses in the semester return 0
 * @param semester - the semester to sum the points in
 * @return the total sum of grades  of effective courses multiple by the course point and multiply by 2
 */
int semesterGetEffectiveGradeSumX2(Semester semester);

/**
 * semesterPrintInfo - Prints a line containing semester details.
 * @param semester - the semester to print it's info
 * @param output_channel - the channel to print the grades to
 * @return
 * SEMESTER_NULL_ARGUMENT - if one of the argument is NULL
 * SEMESTER_OUT_OF_MEMORY - if there was a memory error
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterPrintInfo(Semester semester, FILE* output_channel);

/**
 * semesterDestroy - deallocate all the data of the semester
 * @param semester - the semester to destroy
 * if semester is NULL nothing will be done
 */
void semesterDestroy(SetElement semester);

#endif //EX3_SEMESTER_H
