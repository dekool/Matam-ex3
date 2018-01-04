#include "semester.h"
#include <stdlib.h>
#include "assert.h"
#include "student.h"

typedef struct semester_t {
    int semester_number;
    List grades;
} semester_t;

/**
 * semesterCreate - creates a new semester grade
 * @param semester - the semester number. must be positive number
 * @return
 * SEMESTER_OUT_OF_MEMORY - if there was a memory error
 * SEMESTER_INVALID_PARAMETER - if semester is not valid (negative number)
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterCreate(int semester_number, Semester *semester) {
    if (semester_number <= 0) return SEMESTER_INVALID_PARAMETER;

    Semester new_semester = (Semester) malloc(sizeof(*new_semester));
    if (new_semester == NULL) return SEMESTER_OUT_OF_MEMORY;
    new_semester->semester_number = semester_number;
    new_semester->grades = listCreate(gradeCopy, gradeDestroy);
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
SetElement semesterCopy(SetElement semester) {
    if (semester == NULL || ((Semester)semester)->grades == NULL) return NULL;
    Semester new_semester = (Semester) malloc(sizeof(*new_semester));
    if (new_semester == NULL) return NULL;
    new_semester->semester_number = ((Semester)semester)->semester_number;
    new_semester->grades = listCopy(((Semester)semester)->grades);
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
int semesterCompare(SetElement semester1, SetElement semester2) {
    assert(semester1 != NULL && semester2 != NULL);
    if (((Semester)semester1)->semester_number > ((Semester)semester2)->semester_number) return 1;
    if (((Semester)semester1)->semester_number == ((Semester)semester2)->semester_number) return 0;
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
        if (semester->semester_number == semester_number) {
            return semester;
        }
    }
    return NULL;
}

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
SemesterResult semesterAddGrade(Semester semester, int course_id, char* points, int grade) {
    if (semester == NULL || semester->grades == NULL) {
        return SEMESTER_OUT_OF_MEMORY;
    }
    Grade new_grade = NULL;
    GradeResult create_result = gradeCreate(course_id, points, grade, semester->semester_number, &new_grade);
    if (create_result == GRADE_OUT_OF_MEMORY) return SEMESTER_OUT_OF_MEMORY;
    if (create_result == GRADE_INVALID_PARAMETER) return SEMESTER_INVALID_PARAMETER;

    ListResult add_result = listInsertLast(semester->grades, (ListElement)new_grade);
    gradeDestroy(new_grade);
    if (add_result == LIST_OUT_OF_MEMORY) return SEMESTER_OUT_OF_MEMORY;
    return SEMESTER_OK;
}

/**
 * semesterGetCourseLastGradeObject - get the last grade object inserted for course with the given course_id for the given
 * semester.
 * if there are no grades for this course in this semester NULL is returned
 * @param semester - the semester to search the course grade in
 * @param course_id - the id of the course to search
 * @return
 * the last grade object inserted for the given course in the given semester, or NULL if such grade do not exist
 */
Grade semesterGetCourseLastGradeObject(Semester semester, int course_id) {
    if (semester == NULL || semester->grades == NULL) return NULL;
    Grade last_grade = NULL;

    Grade currentGrade = listGetFirst(semester->grades);
    while (currentGrade != NULL) {
        if (isGradeIsForCourse(currentGrade, course_id)) {
            last_grade =currentGrade;
        }
        currentGrade = listGetNext(semester->grades);
    }
    return last_grade;
}

/**
 * semesterGetCourseLastGrade - get the last grade inserted for course with the given course_id for the given semester.
 * if there are no grade for this course in this semester -1 is returned
 * @param semester - the semester to search the course grade in
 * @param course_id - the id of the course to search
 * @return
 * the last grade inserted for the given course in the given semester, or -1 if such grade do not exist
 */
int semesterGetCourseLastGrade(Semester semester, int course_id) {
    if (semester == NULL || semester->grades == NULL) return -1;
    return getGradeNumber(semesterGetCourseLastGradeObject(semester, course_id));
}

/**
 * semesterGetCourseBestGrade - get the best grade inserted for course with the given course_id for the given
 * semester.
 * if there are no grades for this course in this semester -1 is returned
 * @param semester - the semester to search the course grade in
 * @param course_id - the id of the course to search
 * @return
 * the best grade inserted for the given course in the given semester, or -1 if such grade do not exist
 */
int semesterGetCourseBestGrade(Semester semester, int course_id) {
    if (semester == NULL || semester->grades == NULL) return -1;
    int best_grade = -1;

    Grade currentGrade = listGetFirst(semester->grades);
    while (currentGrade != NULL) {
        if (isGradeIsForCourse(currentGrade, course_id)) {
            if (getGradeNumber(currentGrade) > best_grade) {
                best_grade = getGradeNumber(currentGrade);
            }
        }
        currentGrade = listGetNext(semester->grades);
    }
    return best_grade;
}

/**
 * semesterGetCoursePointsX2 - get the points of the course with the given course_id multiply by 2.
 * if there are no grades for this course in this semester -1 is returned
 * @param semester - the semester to search the course points in
 * @param course_id - the id of the course to search
 * @return
 * the points of the given course in the given semester, or -1 if such grade do not exist
 */
int semesterGetCoursePointsX2(Semester semester, int course_id) {
    if (semester == NULL || semester->grades == NULL) return -1;
    return getCoursePointsX2(semesterGetCourseLastGradeObject(semester, course_id));
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
    Grade currentGrade = (Grade)listGetFirst(semester->grades);
    int number_of_grades = 0;
    while (currentGrade != NULL) {
        if (isGradeIsForCourse(currentGrade, course_id)) {
            number_of_grades++;
        }
        currentGrade = (Grade)listGetNext(semester->grades);
    }
    if (number_of_grades == 0) return  SEMESTER_COURSE_DOES_NOT_EXIST;

    //remove the last one
    currentGrade = (Grade)listGetFirst(semester->grades);
    while (currentGrade != NULL) {
        if (isGradeIsForCourse(currentGrade, course_id)) {
            number_of_grades-=1;
            if (number_of_grades == 0) {
                listRemoveCurrent(semester->grades);
                break;
            }
        }
        currentGrade = (Grade)listGetNext(semester->grades);
    }
    if (listGetSize(semester->grades) == 0) return SEMESTER_GOT_EMPTY;
    return SEMESTER_OK;
}

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
SemesterResult semesterUpdateGrade(Semester semester, int course_id, int new_grade) {
    if (semester == NULL || semester->grades == NULL) return SEMESTER_OUT_OF_MEMORY;
    Grade last_grade = semesterGetCourseLastGradeObject(semester, course_id);
    GradeResult update_result = gradeUpdateGradeNumber(last_grade, new_grade);
    if (update_result == GRADE_NULL_ARGUMENT) return SEMESTER_COURSE_DOES_NOT_EXIST;
    if (update_result == GRADE_INVALID_PARAMETER) return SEMESTER_INVALID_PARAMETER;
    return SEMESTER_OK;
}

/**
 * semesterPrintAllSemesterGrades - prints all the grades for all courses in the given semester,
 * into the given output channel. the grades are printed in the order they were inserted
 * @param semester - the semester to print it's grades
 * @param output_channel - the channel to print the grades to
 */
void semesterPrintAllSemesterGrades(Semester semester, FILE* output_channel) {
    if (semester == NULL || semester->grades == NULL) return;
    LIST_FOREACH(Grade, current_grade, semester->grades) {
        gradePrintInfo(current_grade, output_channel);
    }
}

/**
 * semesterGetCoursesSet - get a set of ids of all the courses in the given semester.
 * if there are no courses in the given semester or there was a memory error, set is not changed
 * @param semester - the semester to search the courses in
 * @param set - pointer to the set that will be returned
 * @return
 * SEMESTER_OUT_OF_MEMORY - if there was a memory error
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterGetCoursesSet(Semester semester, Set *set) {
    if (semester == NULL || semester->grades == NULL) return SEMESTER_OUT_OF_MEMORY;
    Set new_set = setCreate(copyInt, destroyInt, compareInt);
    if (new_set == NULL) return SEMESTER_OUT_OF_MEMORY;
    int current_course_id;
    LIST_FOREACH(Grade, current_grade, semester->grades) {
        current_course_id = getCourseId(current_grade);
        SetResult add_result = setAdd(new_set, &current_course_id);
        if (add_result == SET_OUT_OF_MEMORY) {
            setDestroy(new_set);
            return SEMESTER_OUT_OF_MEMORY;
        }
    }
    *set = new_set;
    return SEMESTER_OK;
}

/**
 * semesterGetTotalCoursePointsX2 - return the total sum of course points multiple by 2 of the courses in the semester.
 * if the semester is NULL return -1, and if there are no courses in the semester return 0
 * @param semester - the semester to sum the points in
 * @return the total sum of course points multiple by 2
 */
int semesterGetTotalCoursePointsX2(Semester semester) {
    if (semester == NULL || semester->grades == NULL) return -1;
    int total_course_points = 0;
    LIST_FOREACH(Grade, current_grade, semester->grades) {
        total_course_points += getCoursePointsX2(current_grade);
    }
    return total_course_points;
}

/**
 * semesterGetFailedCoursePointsX2 - return the total sum of course points multiple by 2 of the failed courses in the
 * semester.
 * if the semester is NULL return -1, and if there are no courses in the semester return 0
 * @param semester - the semester to sum the points in
 * @return the total sum of failed course points multiple by 2
 */
int semesterGetFailedCoursePointsX2(Semester semester) {
    if (semester == NULL || semester->grades == NULL) return -1;
    int total_failed_course_points = 0;
    int current_grade_number;
    LIST_FOREACH(Grade, current_grade, semester->grades) {
        current_grade_number = getGradeNumber(current_grade);
        if (current_grade_number < 55) {
            total_failed_course_points += getCoursePointsX2(current_grade);
        }
    }
    return total_failed_course_points;
}

/**
 * semesterGetEffectiveCoursePointsX2 - return the total sum of effective course points multiple by 2 of the courses
 * in the semester.
 * if the semester is NULL or there was memory error return -1, and if there are no courses in the semester return 0
 * @param semester - the semester to sum the points in
 * @return the total sum of effective course points multiple by 2
 */
int semesterGetEffectiveCoursePointsX2(Semester semester) {
    if (semester == NULL || semester->grades == NULL) return -1;
    Set courses_set;
    SemesterResult create_set_result = semesterGetCoursesSet(semester, &courses_set);
    if (create_set_result == SEMESTER_OUT_OF_MEMORY) return -1;
    int total_effective_course_points = 0;
    Grade current_course_grade;
    SET_FOREACH(int*, current_course_id, courses_set) {
        current_course_grade = semesterGetCourseLastGradeObject(semester, *current_course_id);
        total_effective_course_points += getCoursePointsX2(current_course_grade);
    }
    setDestroy(courses_set);
    return total_effective_course_points;
}

/**
 * semesterGetEffectiveGradeSumX2 - return the total sum of grades of the effective courses in the semester, multiply
 * by the course points and multiple by 2
 * if the semester is NULL or there was memory error return -1, and if there are no courses in the semester return 0
 * @param semester - the semester to sum the points in
 * @return the total sum of grades  of effective courses multiple by the course point and multiply by 2
 */
int semesterGetEffectiveGradeSumX2(Semester semester) {
    if (semester == NULL || semester->grades == NULL) return -1;
    Set courses_set;
    SemesterResult create_set_result = semesterGetCoursesSet(semester, &courses_set);
    if (create_set_result == SEMESTER_OUT_OF_MEMORY) return -1;
    int sum_course_points = 0;
    Grade current_course_grade;
    SET_FOREACH(int*, current_course_id, courses_set) {
        current_course_grade = semesterGetCourseLastGradeObject(semester, *current_course_id);
        sum_course_points += (getGradeNumber(current_course_grade)*getCoursePointsX2(current_course_grade));
    }
    setDestroy(courses_set);
    return sum_course_points;
}

/**
 * semesterPrintInfo - Prints a line containing semester details.
 * @param semester - the semester to print it's info
 * @param output_channel - the channel to print the grades to
 * @return
 * SEMESTER_NULL_ARGUMENT - if one of the argument is NULL
 * SEMESTER_OUT_OF_MEMORY - if there was a memory error
 * SEMESTER_OK - otherwise
 */
SemesterResult semesterPrintInfo(Semester semester, FILE* output_channel) {
    if (semester == NULL) return SEMESTER_NULL_ARGUMENT;
    if (semester->grades == NULL) return SEMESTER_OUT_OF_MEMORY;
    int total_course_points_x2 = semesterGetTotalCoursePointsX2(semester);
    if (total_course_points_x2 == -1) return SEMESTER_OUT_OF_MEMORY;
    int total_failed_course_points_x2 = semesterGetFailedCoursePointsX2(semester);
    if (total_failed_course_points_x2 == -1) return SEMESTER_OUT_OF_MEMORY;
    int total_effective_course_points_x2 = semesterGetEffectiveCoursePointsX2(semester);
    if (total_effective_course_points_x2 == -1) return SEMESTER_OUT_OF_MEMORY;
    int sum_effective_course_grades = semesterGetEffectiveGradeSumX2(semester);
    if (sum_effective_course_grades == -1) return SEMESTER_OUT_OF_MEMORY;
    mtmPrintSemesterInfo(output_channel, semester->semester_number, total_course_points_x2,
                         total_failed_course_points_x2, total_effective_course_points_x2, sum_effective_course_grades);
    return SEMESTER_OK;
}

/**
 * semesterDestroy - deallocate all the data of the semester
 * @param semester - the semester to destroy
 * if semester is NULL nothing will be done
 */
void semesterDestroy(SetElement semester) {
    if (semester == NULL) return;
    listDestroy(((Semester)semester)->grades);
    free((Semester)semester);
}
