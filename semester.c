#include "semester.h"
#include <string.h>
#include <stdlib.h>
#include "assert.h"
#include "grade.h"

typedef struct semester_t {
    int semester;
    List grades;
};

/**
 * semesterCreate - creates a new semester grade
 * @param semester - the semester number. must be positive number
 * @return
 * SEMESTER_OUT_OF_MEMORY - if there was a memory error
 * SEMESTER_INVALID_PARAMETER - if semester is not valid (negative number)
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterCreate(int semester_number, Semester *semester) {
    if (semester < 0) return SEMESTER_INVALID_PARAMETER;

    Semester new_semester = (Semester) malloc(sizeof(*new_semester));
    if (new_semester == NULL) return SEMESTER_OUT_OF_MEMORY;
    new_semester->semester = semester_number;
    new_semester->grades = listCreate((ListElement)gradeCopy, (ListElement)gradeDestroy);
    if (new_semester->grades == NULL) {
        free(new_semester);
        return SEMESTER_OUT_OF_MEMORY;
    }
    *semester = new_semester;
    return SEMESTER_OK;
}

/**
 * semesterCopy - creates a copy of target semester
 * @param semester - target semester
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A Semester with the same data as given semester otherwise.
 */
Semester semesterCopy(Semester semester) {
    if (semester == NULL || semester->grades == NULL) return NULL;
    Semester new_semester = (Semester) malloc(sizeof(*new_semester));
    if (new_semester == NULL) return NULL;
    new_semester->semester = semester->semester;
    new_semester->grades = listCopy(semester->grades);
    if (new_semester->grades == NULL) {
        free(new_semester);
        return NULL;
    }
    return new_semester;
}

/**
 * semesterCompare - compare between 2 semesters by the semester number
 * @param semester1 - first semester to compare
 * @param semester2  - second semester to compare
 * @return
 * 1 - if semester1's semester number is bigger
 * -1 - if semester2's semester number is bigger
 * 0 - if both semesters has the same semester number
 */
int semesterCompare(Semester semester1, Semester semester2) {
    assert(semester1 != NULL && semester2 != NULL);
    if (semester1->semester > semester2->semester) return 1;
    if (semester1->semester == semester2->semester) return 0;
    // only option left - semester2's id is bigger
    return -1;
}

/**
 * getSemesterFromSet - return the semester pointer of the semester with given semester from given set.
 * @param set - the set to search the semester in
 * @param semester - the semester of the semester to search
 * @return
 * pointer to the semester with the given semester (if found).
 * if the semester does not found, returns NULL
 */
Semester getSemesterFromSet(Set set, int semester_number) {
    SET_FOREACH(Semester, semester, set) {
        if (semester->semester == semester_number) {
            return semester;
        }
    }
    return NULL;
}

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
SemesterResult semesterAddGrade(Semester semester, int course_id, char* points, int grade) {
    assert(semester != NULL && semester->grades != NULL);
    Grade new_grade = NULL;
    GradeResult create_result = gradeCreate(course_id, points, grade, &new_grade);
    if (create_result == GRADE_OUT_OF_MEMORY) return SEMESTER_OUT_OF_MEMORY;
    if (create_result == GRADE_INVALID_PARAMETER) return SEMESTER_INVALID_PARAMETER;

    ListResult add_result = listInsertLast(semester->grades, new_grade);
    gradeDestroy(new_grade);
    if (add_result == LIST_OUT_OF_MEMORY) return SEMESTER_OUT_OF_MEMORY;
    return SEMESTER_OK;
}

/**
 * semesterRemoveGrade - removes the last grade of the course with the given course id
 * @param semester - the semester to remove the grade from
 * @param course_id - the id of the course to remove the grade from
 * @return
 * SEMESTER_OUT_OF_MEMORY - if there was a memory error
 * SEMESTER_COURSE_DOES_NOT_EXIST -if there are no grades for course with the given id in this semester
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterRemoveGrade(Semester semester, int course_id) {
    List course_grade_list = listFilter(semester->grades, isGradeIsForCourse, &course_id);
    if (course_grade_list == NULL) return SEMESTER_COURSE_DOES_NOT_EXIST;
    int number_of_grades = listGetSize(course_grade_list);

    Grade currentGrade = listGetFirst(semester->grades);
    while (currentGrade != NULL) {
        if (isGradeIsForCourse(currentGrade, &course_id)) {
            number_of_grades-=1;
            if (number_of_grades == 0) {
                listRemoveCurrent(semester->grades);
            }
        }
        currentGrade = listGetNext(semester->grades);
    }
    return SEMESTER_OK;
}

/**
 * semesterDestroy - deallocate all the data of the semester
 * @param semester - the semester to destroy
 * if semester is NULL nothing will be done
 */
void semesterDestroy(Semester semester) {
    if (semester == NULL) return;
    listDestroy(semester->grades);
    free(semester);
}
