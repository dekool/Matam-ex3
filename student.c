#include "student.h"
#include <string.h>
#include <stdlib.h>
#include "assert.h"
#include "semester.h"
#include "grade.h"

#define MIN_SPORT_COURSE_ID 390000
#define MAX_SPORT_COURSE_ID 399999

typedef struct student_t {
    int id;
    char* firstName;
    char* lastName;
    Set friends; // set of the students' ids
    Set pendingFriendRequests; // set of the students' ids
    Set semesters;
};

/**
* studentCreate: creates new student (and allocates memory for all it's details).
* @param id - the id of the student
* @param firstName - the student's first name. the data is copied from the pointer
* @param lastName - the student's last name. the data is copied from the pointer
* @param student - pointer to the student object which the new student will be saved to
* @return
* STUDENT_NULL_ARGUMENT - if one of the argument is Null
* STUDENT_OUT_OF_MEMORY - if there was a memory error
* STUDENT_INVALID_PARAMETER - if the id entered is invalid (number is higher than 999999999 or negative)
* STUDENT_OK - otherwise
*/
StudentResult studentCreate(int id, char* firstName, char* lastName, Student *student) {
    if (firstName == NULL || lastName == NULL) return STUDENT_NULL_ARGUMENT;
    if (id >= 1000000000) return STUDENT_INVALID_PARAMETER;

    Student new_student = (Student) malloc(sizeof(*new_student));
    if (new_student == NULL) return STUDENT_OUT_OF_MEMORY;
    new_student->id = id;
    new_student->firstName = (char*) malloc(strlen(firstName) + 1);
    if (new_student->firstName == NULL) {
        free(new_student);
        return STUDENT_OUT_OF_MEMORY;
    }
    strcpy(new_student->firstName, firstName);
    new_student->lastName = (char*) malloc(strlen(lastName) + 1);
    if (new_student->lastName == NULL) {
        free(new_student->firstName);
        free(new_student);
        return STUDENT_OUT_OF_MEMORY;
    }
    strcpy(new_student->lastName, lastName);
    new_student->friends = setCreate(copyInt, destroyInt, compareInt);
    if (new_student->friends == NULL) {
        free(new_student->firstName);
        free(new_student->lastName);
        free(new_student);
        return STUDENT_OUT_OF_MEMORY;
    }
    new_student->pendingFriendRequests = setCreate(copyInt, destroyInt, compareInt);
    if (new_student->pendingFriendRequests == NULL) {
        free(new_student->firstName);
        free(new_student->lastName);
        setDestroy(new_student->friends);
        free(new_student);
        return STUDENT_OUT_OF_MEMORY;
    }
    new_student->semesters = setCreate((SetElement)semesterCopy, (SetElement)semesterDestroy, (SetElement)semesterCompare);
    if (new_student->semesters == NULL) {
        free(new_student->firstName);
        free(new_student->lastName);
        setDestroy(new_student->friends);
        setDestroy(new_student->pendingFriendRequests);
        free(new_student);
        return STUDENT_OUT_OF_MEMORY;
    }
    *student = new_student;
    return STUDENT_OK;
}

/**
 * studentCopy: Creates a copy of target student.
 *
 * @param student - Target student.
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A Student with the same data as given student otherwise.
 */
Student studentCopy(Student student) {
    if (student == NULL) return NULL;
    Student new_student = (Student) malloc(sizeof(*new_student));
    if (new_student == NULL) return NULL;
    new_student->id = student->id;
    new_student->firstName = (char*) malloc(strlen(student->firstName) + 1);
    if (new_student->firstName == NULL) {
        free(new_student);
        return NULL;
    }
    strcpy(new_student->firstName, student->firstName);
    new_student->lastName = (char*) malloc(strlen(student->lastName) + 1);
    if (new_student->lastName == NULL) {
        free(new_student->firstName);
        free(new_student);
        return NULL;
    }
    strcpy(new_student->lastName, student->lastName);
    new_student->friends = setCopy(student->friends);
    if (new_student->friends == NULL) {
        free(new_student->firstName);
        free(new_student->lastName);
        free(new_student);
        return NULL;
    }
    new_student->pendingFriendRequests = setCopy(student->pendingFriendRequests);
    if (new_student->pendingFriendRequests == NULL) {
        free(new_student->firstName);
        free(new_student->lastName);
        setDestroy(new_student->friends);
        free(new_student);
        return NULL;
    }
    new_student->semesters = setCopy(student->semesters);
    if (new_student->semesters == NULL) {
        free(new_student->firstName);
        free(new_student->lastName);
        setDestroy(new_student->friends);
        setDestroy(new_student->pendingFriendRequests);
        free(new_student);
        return NULL;
    }
    return new_student;
}

/**
 * studentCompare - compare between 2 students by their id number
 *
 * @param student1 - first student to compare
 * @param student2 - second student to compare
 * @return
 * 1 if student1's id is bigger
 * -1 if student2's id is bigger
 * 0 if both student ids are the same
 */
int studentCompare(Student student1, Student student2) {
    assert (student1 != NULL && student2 != NULL);
    if (student1->id == student2->id) {
        return 0;
    }
    if (student1->id > student2->id) {
        return 1;
    }
    // only option left - student2's id is bigger
    return -1;
}

///////////////////////////////////////////////////////////////////
// functions to use for Set of ints (students' ids)
///////////////////////////////////////////////////////////////////
/**
 * copyInt - copies int
 * @param element - int pointer to copy
 * @return copied int. NULL if memory error
 */
SetElement copyInt(SetElement element) {
    if (element == NULL) return NULL;
    int* newInt = malloc(sizeof(int));
    if (newInt == NULL) return NULL;
    *newInt = *(int*)element;
    return newInt;
}
/**
 * compareInt - compares between 2 ints
 * @param element1 - pointer to int1 to compare
 * @param element2 - pointer to int2 to compare
 * @return
 * 0 if the ints are equal.
 * 1 if the left int is bigger
 * -1 if the right int is bigger
 */
int compareInt(SetElement element1, SetElement element2) {
    if (*(int*)element1 == *(int*)element2) {
        return 0;
    }
    if (*(int*)element1 > *(int*)element2) {
        return 1;
    }
    // only option left - element2 is bigger
    return -1;
}

/**
 * destroyInt - free the pointer to the int
 * @param element - pointer to int
 */
void destroyInt(SetElement element) {
    free(element);
}
////////////////////////////////////////////////////////////////////

/**
 * getStudentFromSet - return the student object of the student with given id from given set.
 * @param set - the set to search the student in
 * @param id - the id of the student to search
 * @return
 * the student with the given id (if found).
 * if the student does not found, returns NULL
 */
Student getStudentFromSet(Set set, int id) {
    SET_FOREACH(Student, student, set) {
        if (student->id == id) {
            return student;
        }
    }
    return NULL;
}

/**
 * studentGetStudentFriends - return set of ids of the student's friends.
 * @param student - the to search for his friends
 * @return
 * set of ids of the student's friends.
 * if the student is NULL, returns NULL
 */
Set studentGetStudentFriends(Student student) {
    if (student == NULL) return NULL;
    return student->friends;
}

/**
 * addFriendRequest - add friend request from student to friend
 * @param student - the student who send the request
 * @param friend - the student who gets the request
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_NULL_ARGUMENT - if one of the arguments is NULL
 * STUDENT_ALREADY_FRIEND - if the students are already friends (or they are the same student)
 * STUDENT_ALREADY_REQUESTED - if there is already a friend request from student to friend
 * STUDENT_OK - otherwise (success)
 */

StudentResult addFriendRequest(Student student, Student friend) {
    if (student == NULL || friend == NULL || student->friends == NULL) return STUDENT_NULL_ARGUMENT;
    if (setIsIn(student->friends, &(friend->id)) || studentCompare(student, friend)) {
        return STUDENT_ALREADY_FRIEND;
    }
    SetResult addResult = setAdd(friend->pendingFriendRequests, &(student->id));
    if (addResult == SET_OUT_OF_MEMORY) return STUDENT_OUT_OF_MEMORY;
    if (addResult == SET_ITEM_ALREADY_EXISTS) return STUDENT_ALREADY_REQUESTED;
    return STUDENT_OK;
}

/**
 * removeFriendRequest - remove a friend request from student to friend
 * @param student - the student who sent the request
 * @param friend - the student who got the request
 * @return
 * STUDENT_REQUEST_NOT_EXIST - if there was no friend request from student to friend
 * STUDENT_OK - otherwise (success)
 */
StudentResult removeFriendRequest(Student student, Student friend) {
    SetResult removeResult = setRemove(friend->pendingFriendRequests, &(student->id));
    if (removeResult == SET_ITEM_DOES_NOT_EXIST) return STUDENT_REQUEST_NOT_EXIST;
    return STUDENT_OK;
}

/**
 * isThereFriendRequest - checks if there is a pending friend request from student to friend
 * @param student - the student who sent the request
 * @param friend - the student who got the request
 * @return whether there is such request or not (or they are the same student)
 */
bool isThereFriendRequest(Student student, Student friend) {
    return (setIsIn(student->pendingFriendRequests, &(friend->id)) || studentCompare(student, friend) == 0);
}

/**
 * addFriend - add student2 to student1 friends list
 *
 * @param student1 - the student to add the friend to it's list
 * @param student2 - the student to be added as friend
 * @return
 * STUDENT_NULL_ARGUMENT - if one of the argument is Null
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_ALREADY_FRIEND - if friend if already in student's friend list (or they are the same student)
 * STUDENT_OK - otherwise
 */
StudentResult addFriend(Student student, Student friend) {
    if (student == NULL || friend == NULL) return STUDENT_NULL_ARGUMENT;
    if (studentCompare(student, friend) == 0) return STUDENT_ALREADY_FRIEND;
    SetResult result = setAdd(student->friends, &(friend->id));
    if (result == SET_OUT_OF_MEMORY) return STUDENT_OUT_OF_MEMORY;
    if (result == SET_ITEM_ALREADY_EXISTS) return STUDENT_ALREADY_FRIEND;
    return STUDENT_OK;
}

/**
 * isFriend - checks if friend is in the friends list of student
 * @param student - the student to check it's friend list
 * @param friend - the student to check if he is in the friends list
 * @return
 * true if friend is in student's friends list (or they are the same student), false otherwise
 */
bool isFriend(Student student, Student friend) {
    return (setIsIn(student->friends, &(friend->id)) || studentCompare(student, friend) == 0);
}

/**
 * unFriend - remove student1 from student2's friends list and vise versa.
 * @param student1 - the first student
 * @param student2 - the second student
 */
void removeFriend(Student student1, Student student2) {
    setRemove(student1->friends, &(student2->id));
    setRemove(student2->friends, &(student1->id));
}

/**
 * removeStudentFromFriendsSet - remove the student details from all other students' friends details, and from other
 * student's friends requests
 * @param set - set of students
 * @param student - the student to remove
 */
void removeStudentFromFriendsSet(Set set, Student student) {
    // remove student from all friend lists
    SET_FOREACH(Student, currentStudent, set) {
        setRemove(currentStudent->friends, &(student->id));
    }
    // remove all student's friend requests
    SET_FOREACH(Student, currentStudent, set) {
        setRemove(currentStudent->pendingFriendRequests, &(student->id));
    }
}

/**
 * studentAddGrade - adds the given grade to the student's grade sheet, attacked to the given course
 * in the given semester.
 * @param student - the student to add the grade to
 * @param semester_number - the semester number the grade is attached to (must be positive number)
 * @param course_id - the id of the course the grade is attached to (must be positive number lower than 1000000000)
 * @param points - the number of points for the course the grade is attached to
 * (must be in the format of X or X.0 or X.5, while X is an array of digits)
 * @param grade - the grade to add to the grade sheet (must be number between 0 and 100)
 * @return
 * STUDENT_NULL_ARGUMENT - if points is NULL
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_INVALID_PARAMETER - if one of the parameters is not valid
 * STUDENT_OK - otherwise
 */
StudentResult studentAddGrade(Student student, int semester_number, int course_id, char* points, int grade) {
    if (points == NULL || student == NULL) return STUDENT_NULL_ARGUMENT;
    Semester semester = getSemesterFromSet(student->semesters, semester_number);
    int semester_added = 0; // in case we will have to remove it later in case of error
    // if the semester do not exist - first create it
    if (semester == NULL) {
        SemesterResult create_result = semesterCreate(semester_number, &semester);
        if (create_result == SEMESTER_OUT_OF_MEMORY) return STUDENT_OUT_OF_MEMORY;
        if (create_result == SEMESTER_INVALID_PARAMETER) return STUDENT_INVALID_PARAMETER;
        SetResult add_result = setAdd(student->semesters, semester);
        if (add_result == SET_OUT_OF_MEMORY) {
            semesterDestroy(semester);
            return STUDENT_OUT_OF_MEMORY;
        }
        semester_added = 1;
    }
    SemesterResult add_result = semesterAddGrade(semester, course_id, points, grade);
    if (add_result == SEMESTER_OUT_OF_MEMORY) {
        if (semester_added == 1) {
            setRemove(student->semesters, semester);
            semesterDestroy(semester);
            return STUDENT_OUT_OF_MEMORY;
        }
    }
    if (add_result == SEMESTER_INVALID_PARAMETER) {
        if (semester_added == 1) {
            setRemove(student->semesters, semester);
            semesterDestroy(semester);
            return STUDENT_INVALID_PARAMETER;
        }
    }
    semesterDestroy(semester); // the data is copied to the set
    return STUDENT_OK;
}

/**
 * studentRemoveGrade - removes the last grade of the course with the given id in the given semester of the given
 * student
 * @param student - the student to remove his grade
 * @param semester - the semester which the grade to remove is in
 * @param course_id - the id of the course of the grade to remove
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_COURSE_DOES_NOT_EXIST -if there are no grades for the given student in the given course at the given semester
 * STUDENT_OK - otherwise
 */
StudentResult studentRemoveGrade(Student student, int semester, int course_id) {
    assert(student != NULL && student->semesters != NULL);
    Semester grade_semester = getSemesterFromSet(student->semesters, semester);
    if (grade_semester == NULL) return STUDENT_COURSE_DOES_NOT_EXIST;
    SemesterResult remove_result = semesterRemoveGrade(grade_semester, course_id);
    if (remove_result == SEMESTER_COURSE_DOES_NOT_EXIST) return STUDENT_COURSE_DOES_NOT_EXIST;
    if (remove_result == SEMESTER_OUT_OF_MEMORY) return STUDENT_OUT_OF_MEMORY;
    return STUDENT_OK;
}

/**
 * studentUpdateGrade - updates the grade for the given student in the last semester he have grade for the course with
 * the given course id.
 * @param student - the student to update the grade for
 * @param course_id - the id of the course the grade to update is attached to
 * @param new_grade - the new grade for the course (must be integer between 0 and 100)
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_COURSE_DOES_NOT_EXIST -if there are no grades for the given student in the given course at the given semester
 * STUDENT_INVALID_PARAMETER - if the new_grade is not valid
 * STUDENT_OK - otherwise
 */
StudentResult studentUpdateGrade(Student student, int course_id, int new_grade) {
    assert(student != NULL && student->semesters != NULL);
    // get the last semester the student have grade for this course
    Semester max_semester = NULL;
    SET_FOREACH(Semester, current_semester, student->semesters) {
        if (semesterGetCourseLastGrade(current_semester, course_id) != -1) {
            max_semester = current_semester;
        }
    }
    if (max_semester == NULL) return STUDENT_COURSE_DOES_NOT_EXIST;
    SemesterResult update_result = semesterUpdateGrade(max_semester, course_id, new_grade);
    if (update_result == SEMESTER_INVALID_PARAMETER) return STUDENT_INVALID_PARAMETER;
    if (update_result == SEMESTER_COURSE_DOES_NOT_EXIST) return STUDENT_COURSE_DOES_NOT_EXIST;
    return STUDENT_OK;
}

/**
 * studentGetAllCoursesSet - inner function to get a set of the ids of all the courses the student have grades in
 * @param student - the student to get his courses
 * @param set - pointer to the set that will be return
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_OK - otherwise
 */
static StudentResult studentGetAllCoursesSet(Student student, Set *set) {
    assert(student != NULL && student->semesters != NULL);
    Set courses = setCreate(copyInt, destroyInt, compareInt);
    if (courses == NULL) return STUDENT_OUT_OF_MEMORY;
    Set semester_courses;
    SET_FOREACH(Semester, current_semester, student->semesters) {
        SemesterResult semester_courses_result = semesterGetCoursesSet(current_semester, &semester_courses);
        if (semester_courses_result == SEMESTER_OUT_OF_MEMORY) {
            setDestroy(courses);
            return STUDENT_OUT_OF_MEMORY;
        }
        SET_FOREACH(int, current_semester_course, semester_courses) {
            // try to add the course even if it is already in the set - because if it is, nothing will happened
            SetResult add_course_result = setAdd(courses, &current_semester_course);
            if (add_course_result == SET_OUT_OF_MEMORY) {
                setDestroy(courses);
                setDestroy(semester_courses);
                return STUDENT_OUT_OF_MEMORY;
            }
        }
    }
    *set = courses;
    return STUDENT_OK;
}

/**
 * isCourseASportCourse - inner function to determine if a course is a sport course
 * @param course_id - the id of the course
 * @return whether this course is a sport course
 */
static bool isCourseASportCourse(int course_id) {
    return (course_id >= MIN_SPORT_COURSE_ID && course_id <= MAX_SPORT_COURSE_ID);
}

/**
 * addEffectiveSheetCourseGradeAndPointsX2 - inner function that adds the effective sheet grade and point (multiple
 * by 2) of the given course of the given student to the sum of the effective grades and point (given as pointers).
 * if it is a course sport - add every semester effective grade and points
 * @param student - the student to calculate it's grades
 * @param course_id - the id of the course to add it's grades
 * @param total_effective_course_points_x2 - pointer to the sum of effective points of the courses so far(multiple by 2)
 * @param sum_effective_course_grades - pointer to the sum of effective grades of the courses so far
 */
static void addEffectiveSheetCourseGradeAndPointsX2(Student student, int course_id,
                                         int* total_effective_course_points_x2, int* sum_effective_course_grades) {
    assert(student != NULL && sum_effective_course_grades != NULL && total_effective_course_points_x2 != NULL);

    int course_effective_semester_grades, course_points_x2, course_effective_sheet_grade = 0,course_points_sheet_x2 = 0;
    SET_FOREACH(Semester, current_semester, student->semesters) {
        course_points_x2 = semesterGetCoursePointsX2(current_semester, course_id);
        course_effective_semester_grades = semesterGetCourseLastGrade(current_semester, course_id);
        // check if this course was learned at this semester
        if (course_points_x2 != -1 && course_effective_semester_grades != -1) {
            // if it is sport course - add to total
            if (isCourseASportCourse(course_id)) {
                *total_effective_course_points_x2 += course_points_x2;
                *sum_effective_course_grades += (course_points_x2*course_effective_semester_grades);
            } else { // if it is not a sport course - keep the last semester details
                course_effective_sheet_grade = course_effective_semester_grades;
                course_points_sheet_x2 = course_points_x2;
            }
        }
    }
    // if it is not a sport course - add the course data after checking all semesters
    if (!isCourseASportCourse(course_id)) {
        *total_effective_course_points_x2 += course_points_sheet_x2;
        *sum_effective_course_grades += (course_points_sheet_x2 * course_effective_sheet_grade);
    }
}

/**
 * printEffectiveSheetCourseGradeInfo - inner function that prints the effective sheet grade info of the the with the
 * given id. if it is a sport course- print the effective grade info of every semester this course was learned.
 * @param student - the student to calculate it's grades
 * @param course_id - the id of the course to add it's grades
 * @param output_channel - the channel to print the report to
 */
static void printEffectiveSheetCourseGradeInfo(Student student, int course_id, FILE* output_channel) {
    assert(student != NULL && output_channel != NULL);

    int course_effective_semester_grades, course_points_x2, course_effective_sheet_grade = 0,course_points_sheet_x2 = 0;
    SET_FOREACH(Semester, current_semester, student->semesters) {
        course_points_x2 = semesterGetCoursePointsX2(current_semester, course_id);
        course_effective_semester_grades = semesterGetCourseLastGrade(current_semester, course_id);
        // check if this course was learned at this semester
        if (course_points_x2 != -1 && course_effective_semester_grades != -1) {
            // if it is sport course - print the details of this semester
            if (isCourseASportCourse(course_id)) {
                mtmPrintGradeInfo(output_channel, course_id, course_points_x2, course_effective_semester_grades);
            } else { // if it is not a sport course - keep the last semester details
                course_effective_sheet_grade = course_effective_semester_grades;
                course_points_sheet_x2 = course_points_x2;
            }
        }
    }
    // if it is not a sport course - print the course data after checking all semesters
    if (!isCourseASportCourse(course_id)) {
        mtmPrintGradeInfo(output_channel, course_id, course_points_sheet_x2, course_effective_sheet_grade);
    }
}

/**
 * studentPrintSummary - inner function to print summary of all the given student's grade sheet
 * @param student - the student to print his summary
 * @param output_channel - the channel to print the report to
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_OK - otherwise
 */
static StudentResult studentPrintSummary(Student student, FILE* output_channel) {
    assert(student != NULL && student->semesters != NULL && output_channel != NULL);
    int total_course_points_x2 = 0, total_failed_course_points_x2 = 0, total_effective_course_points_x2 = 0,
            sum_effective_course_grades = 0;
    int semester_course_points_x2, semester_failed_course_points_x2;
    SET_FOREACH(Semester, current_semester, student->semesters) {
        semester_course_points_x2 = semesterGetTotalCoursePointsX2(current_semester);
        if (semester_course_points_x2 == -1) return STUDENT_OUT_OF_MEMORY;
        semester_failed_course_points_x2 = semesterGetFailedCoursePointsX2(current_semester);
        if (semester_failed_course_points_x2 == -1) return STUDENT_OUT_OF_MEMORY;
    }
    Set courses;
    StudentResult set_create_result = studentGetAllCoursesSet(student, &courses);
    if (set_create_result == SET_OUT_OF_MEMORY) return STUDENT_OUT_OF_MEMORY;
    SET_FOREACH(int, current_course_id, courses) {
        addEffectiveSheetCourseGradeAndPointsX2(student, current_course_id, &total_effective_course_points_x2,
                                                      &sum_effective_course_grades);
    }
    setDestroy(courses);
    mtmPrintSummary(output_channel, total_course_points_x2, total_failed_course_points_x2,
                    total_effective_course_points_x2,sum_effective_course_grades);
    return STUDENT_OK;
}

/**
 * studentPrintCleanSummary - inner function to print clean summary of all the given student's grade sheet
 * @param student - the student to print his summary
 * @param output_channel - the channel to print the report to
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_OK - otherwise
 */
static StudentResult studentPrintCleanSummary(Student student, FILE* output_channel) {
    assert(student != NULL && student->semesters != NULL && output_channel != NULL);
    int total_effective_course_points_x2 = 0, sum_effective_course_grades = 0;
    Set courses;
    StudentResult set_create_result = studentGetAllCoursesSet(student, &courses);
    if (set_create_result == SET_OUT_OF_MEMORY) return STUDENT_OUT_OF_MEMORY;
    SET_FOREACH(int, current_course_id, courses) {
        addEffectiveSheetCourseGradeAndPointsX2(student, current_course_id, &total_effective_course_points_x2,
                                                &sum_effective_course_grades);
    }
    setDestroy(courses);
    mtmPrintCleanSummary(output_channel, total_effective_course_points_x2, sum_effective_course_grades);
    return STUDENT_OK;
}

/**
 * studentPrintFullReport - prints full grades report of the student into the given outpt channel
 * @param student - the student to print his report
 * @param output_channel - the channel to print the report to
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_OK - otherwise
 */
StudentResult studentPrintFullReport(Student student, FILE* output_channel) {
    assert(student != NULL && student->semesters != NULL && output_channel != NULL);
    mtmPrintStudentInfo(output_channel, student->id, student->firstName, student->lastName);
    SET_FOREACH(Semester, current_semester, student->semesters) {
        semesterPrintAllSemesterGrades(current_semester, output_channel);
        SemesterResult print_result = semesterPrintInfo(current_semester, output_channel);
        if (print_result == SEMESTER_OUT_OF_MEMORY || print_result == STUDENT_NULL_ARGUMENT) {
            return STUDENT_OUT_OF_MEMORY;
        }
    }
    // print the summary and return it's result (memory error or ok)
    return studentPrintSummary(student, output_channel);
}

/**
 * studentPrintCleanReport - prints grades report of the given student containing the effective grades of all the
 * student's courses, sorted by course id (and also by semester number for sport courses with the same id).
 * the print will be to the given output channel.
 * @param student - the student to print his report
 * @param output_channel - the channel to print the report to
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_OK - otherwise
 */
StudentResult studentPrintCleanReport(Student student, FILE* output_channel) {
    assert(student != NULL && student->semesters != NULL && output_channel != NULL);
    mtmPrintStudentInfo(output_channel, student->id, student->firstName, student->lastName);

    Set courses;
    StudentResult set_create_result = studentGetAllCoursesSet(student, &courses);
    if (set_create_result == SET_OUT_OF_MEMORY) return STUDENT_OUT_OF_MEMORY;
    SET_FOREACH(int, current_course_id, courses) {
        printEffectiveSheetCourseGradeInfo(student, current_course_id, output_channel);
    }
    setDestroy(courses);

    // print the clean summary and return it's result (memory error or ok)
    return studentPrintCleanSummary(student, output_channel);
}

/**
 * insertGradeIntoArrayIfHigher - checks if the given grade if higher than at least one of the grades inside the array.
 * if it is - insert it to the array, sorted.
 * @param array - the array to insert the grade into
 * @param length - the length of the array
 * @param grade - the grade to insert
 */
static void insertGradeIntoArrayIfHigher(Grade** array, int length, Grade *grade) {
    assert(array != NULL && grade != NULL);
    for (int i=length - 1; i >= 0; i--) {
        if (array[i] == NULL) {
            array[i] = grade;
            break;
        } else if (gradeCompare(*grade, *array[i]) > 0) {
            for (int j = 0; j < i; j++) {
                array[j] = array[j + 1];
            }
            array[i] = grade;
            break;
        }
    }
}

/**
 * insertGradeIntoArrayIfLower - checks if the given grade if lower than at least one of the grades inside the array.
 * if it is - insert it to the array, sorted.
 * @param array - the array to insert the grade into
 * @param length - the length of the array
 * @param grade - the grade to insert
 */
static void insertGradeIntoArrayIfLower(Grade** array, int length, Grade *grade) {
    assert(array != NULL && grade != NULL);
    for (int i=length - 1; i >= 0; i--) {
        if (array[i] == NULL) {
            array[i] = grade;
            break;
        } else if (gradeCompare(*grade, *array[i]) < 0) {
            for (int j = 0; j < i; j++) {
                array[j] = array[j + 1];
            }
            array[i] = grade;
            break;
        }
    }
}

/**
 * insertCourseGradeIntoArrayIfFit - insert the pointer to the grade of the course with the given id of the given
 * student into the given array if there is suitable place for it in the array.
 * @param student - the student that owns the grades
 * @param course_id - the id of the course the grades are attached to
 * @param array - the array to insert the grade to
 * @param length - the length of the array
 * @param best - should the array kee[ the best grades, or if false - the worst grades
 */
static void insertCourseGradeIntoArrayIfFit(Student student, int course_id, Grade** array, int length, bool best) {
    assert(array != NULL && student != NULL);
    Grade course_semester_effective_grade, course_effective_grade;
    SET_FOREACH(Semester, current_semester, student->semesters) {
        course_effective_grade = semesterGetCourseLastGradeObject(current_semester, course_id);
        // check if this course was learned at this semester
        if (course_effective_grade != NULL) {
            // if it is sport course - insert the details of this semester into the array
            if (isCourseASportCourse(course_id)) {
                course_effective_grade = semesterGetCourseLastGradeObject(current_semester, course_id);
                if (best) {
                    insertGradeIntoArrayIfHigher(array, length, &course_effective_grade);
                } else {
                    insertGradeIntoArrayIfLower(array, length, &course_effective_grade);
                }
            } else { // if it is not a sport course - keep the last semester details
                course_semester_effective_grade = semesterGetCourseLastGradeObject(current_semester, course_id);
            }
        }
    }
    // if it is not a sport course - insert the course data after checking all semesters
    if (!isCourseASportCourse(course_id)) {
        if (best) {
            insertGradeIntoArrayIfHigher(array, length, &course_semester_effective_grade);
        } else {
            insertGradeIntoArrayIfLower(array, length, &course_effective_grade);
        }
    }
}

/**
 * studentPrintGradesArray - prints the grades in the given array, from last grade to the first one. stop printing if
 * reaching NULL.
 * @param array - the array to print
 * @param length - the length of the array
 * @param output_channel - the output channel to print to
 */
static void studentPrintGradesArray(Grade** array, int length, FILE* output_channel) {
    assert(array != NULL && output_channel != NULL);
    for (int i=length - 1; i <= 0; i--) {
        if (array[i] == NULL) {
            break;
        }
        mtmPrintGradeInfo(output_channel, getCourseId(*array[i]), getCoursePointsX2(*array[i]),
                          getGradeNumber(*array[i]));
    }
}

/**
 * studentPrintBestOrWorstGrades - prints the best/worst (according to the parameter given) effective sheet grades of
 * the given student. the amount of grades printed is given (must be positive number)
 * @param student - the student to print his grades
 * @param amount - the number of grades to print (must be positive number)
 * @param best - true to print the best grades and false to print the worst grades
 * @param output_channel - the channel to print the report to
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_INVALID_PARAMETER - if amount is not valid
 * STUDENT_OK - otherwise
 */
StudentResult studentPrintBestOrWorstGrades(Student student, int amount, bool best, FILE* output_channel) {
    assert(student != NULL && student->semesters != NULL && output_channel != NULL);
    if (amount < 1) return STUDENT_INVALID_PARAMETER;
    Grade** best_grades = malloc(sizeof(Grade*) * amount);
    for (int i = 0; i < amount; i++) {
        best_grades = NULL;
    }
    Set courses;
    StudentResult set_create_result = studentGetAllCoursesSet(student, &courses);
    if (set_create_result == SET_OUT_OF_MEMORY) return STUDENT_OUT_OF_MEMORY;
    SET_FOREACH(int, current_course_id, courses) {
        insertCourseGradeIntoArrayIfFit(student, current_course_id, best_grades, amount, best);
    }
    studentPrintGradesArray(best_grades, amount, output_channel);
    setDestroy(courses);
    free(best_grades);
}

/**
 * studentGetBestGradeInCourse - search for the best grade in all semesters in the course with the given id that the
 * given student have. if the student is NULL or do not have any grade for the course with the given id - return -1.
 * @param student - the student to search his best grade
 * @param course_id - the id of the course to search
 * @return
 * the best grade of the given student in the given course. -1 if not found.
 */
int studentGetBestGradeInCourse(Student student, int course_id) {
    if (student == NULL) return -1;
    int best_grade = -1, best_semester_grade;
    SET_FOREACH(Semester, current_semester, student->semesters) {
        best_semester_grade = semesterGetCourseBestGrade(current_semester, course_id);
        if (best_semester_grade > best_grade) {
            best_grade = best_semester_grade;
        }
    }
    return best_grade;
}

/**
 * studentPrintName - prints the name of the student
 * @param student - the student to print his name
 * @param output_channel - the channel to print the report to
 */
void studentPrintName(Student student, FILE* output_channel) {
    if (student == NULL) return;
    mtmPrintStudentName(output_channel, student->firstName, student->lastName);
}

/**
 * studentDestroy - deallocate all the data of the student
 *
 * @param student - the student object to deallocate
 * If student is NULL nothing will be done
 */
void studentDestroy(Student student) {
    if (student == NULL) return;
    free(student->firstName);
    free(student->lastName);
    setDestroy(student->friends);
    setDestroy(student->pendingFriendRequests);
    free(student);
}