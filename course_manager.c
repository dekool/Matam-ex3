#include "course_manager.h"
#include <stdlib.h>
#include "string.h"
#include "assert.h"

const char * available_requests[] = {
        "cancel_course",
        "register_course",
        "remove_course",
};
const char* faculty_response = "your request was rejected";

typedef struct course_manager_t
{
Set students;
Student logged_student;
} course_manager_t;

/**
 * courseManagerCreate - creates a new empty course manager
 * @return
 * NULL - if allocations failed.
 * 	A new CourseManager in case of success.
 */
CourseManager courseManagerCreate() {
    CourseManager course_manager = (CourseManager) malloc(sizeof(*course_manager));
    if (course_manager == NULL) return NULL;
    course_manager->students = setCreate(studentCopy,studentDestroy,studentCompare);
    if (course_manager->students == NULL) {
        free(course_manager);
        return NULL;
    }
    course_manager->logged_student = NULL;
    return course_manager;
}

/**
 * addStudent - adds new student to the system
 * @param course_manager - the course manager to add the student to
 * @param id - the id number of the student
 * @param firstName - the student's first name
 * @param lastName - the student's last name
 * @return
 * COURSE_MANAGER_STUDENT_ALREADY_EXISTS - if the a student with the same id is already registered in the system
 * COURSE_MANAGER_INVALID_PARAMETERS - if the id entered is invalid (number is higher than 999999999 or negative)
 * COURSE_MANAGER_OUT_OF_MEMORY - if there was a memory error
 */
CourseManagerResult addStudent(CourseManager course_manager, int id, char* firstName, char* lastName) {
    if (firstName == NULL || lastName == NULL || course_manager == NULL || course_manager->students == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    Student student = NULL;
    StudentResult createResult = studentCreate(id, firstName, lastName, &student);
    if (createResult == STUDENT_OUT_OF_MEMORY) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (createResult == STUDENT_INVALID_PARAMETER) {
        studentDestroy(student);
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    SetResult add_result = setAdd(course_manager->students, (SetElement)student);
    printf("add result: %d", add_result);
    studentDestroy(student);
    switch(add_result) {
        case SET_ITEM_ALREADY_EXISTS:
            return COURSE_MANAGER_STUDENT_ALREADY_EXISTS;
        case SET_OUT_OF_MEMORY:
            return COURSE_MANAGER_OUT_OF_MEMORY;
        case SET_NULL_ARGUMENT:
            return COURSE_MANAGER_OUT_OF_MEMORY;
        default:
            return COURSE_MANAGER_OK;;
    }
}

/**
 * removeStudent - removes the student with the given id from the system
 * @param course_manager - the course manager to remove the student from
 * @param id  - the id of the student to be removed
 * @return
 * COURSE_MANAGER_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 */
CourseManagerResult removeStudent(CourseManager  course_manager, int id) {
    if (course_manager == NULL || course_manager->students == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    Student student = getStudentFromSet(course_manager->students, id);
    if (student == NULL) {
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    removeStudentFromFriendsSet(course_manager->students, student);
    if (course_manager->logged_student != NULL && studentCompare(course_manager->logged_student, student) == 0) {
        logOutStudent(course_manager);
    }
    SetResult removeResult = setRemove(course_manager->students, student);
    if (removeResult == SET_ITEM_DOES_NOT_EXIST) return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    return COURSE_MANAGER_OK;
}

/**
 * logInStudent - log in student with the given id
 * @param course_manager - the course manager that the student is logging in to
 * @param id - the id of the student who log in
 * @return
 * COURSE_MANAGER_ALREADY_LOGGED_IN - if a student is already logged in to the system
 * COURSE_MANAGER_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 */
CourseManagerResult logInStudent(CourseManager course_manager, int id) {
    if (course_manager == NULL || course_manager->students == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    if (course_manager->logged_student != NULL) return COURSE_MANAGER_ALREADY_LOGGED_IN;
    Student student = getStudentFromSet(course_manager->students, id);
    if (student == NULL) {
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    course_manager->logged_student = student;
    return COURSE_MANAGER_OK;
}

/**
 * logOutStudent - log out the logged student
 * @param course_manager - the course manager that the student is logging in to
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult logOutStudent(CourseManager course_manager) {
    if (course_manager == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    if (course_manager->logged_student == NULL) {
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    course_manager->logged_student = NULL;
    return COURSE_MANAGER_OK;
}

/**
 * sendFriendRequest - send friend request from the logged student to the student with the given id
 * @param course_manager - the course manager that the student is logging in to
 * @param id - the id of the student to send friend request to
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 * COURSE_MANAGER_ALREADY_FRIEND - if the student with the requested id is already a friend of the logged student
 * (or the requested id is the id of the logged student)
 * COURSE_MANAGER_ALREADY_REQUESTED - if their is already a pending friend request from the logged student to the
 * student with the given id
 */
CourseManagerResult sendFriendRequest(CourseManager course_manager, int id) {
    if (course_manager == NULL || course_manager->students == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    if (course_manager->logged_student == NULL) {
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    Student friend = getStudentFromSet(course_manager->students, id);
    if (friend == NULL) {
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    StudentResult add_result = addFriendRequest(course_manager->logged_student, friend);
    switch (add_result) {
        case STUDENT_ALREADY_FRIEND: return COURSE_MANAGER_ALREADY_FRIEND;
        case STUDENT_ALREADY_REQUESTED: return COURSE_MANAGER_ALREADY_REQUESTED;
        default: return COURSE_MANAGER_OK;
    }
}

/**
 * handleFriendRequest - accept or reject a friend request of the logged student
 * @param course_manager - the course manager that the student is logging in to
 * @param otherId - the id of the student to handle it's request
 * @param action - accept/reject
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 * COURSE_MANAGER_ALREADY_FRIEND - if the student with the requested id is already a friend of the logged student
 * (or the requested id is the id of the logged student)
 * COURSE_MANAGER_NOT_REQUESTED - if there is no pending friend request from the logged student to the
 * student with the given id
 * COURSE_MANAGER_INVALID_PARAMETERS - if the action is not one of "accept" or "reject"
 */
CourseManagerResult handleFriendRequest(CourseManager course_manager, int otherId, char* action) {
    if (course_manager == NULL || course_manager->students == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;

    Student logged_in = course_manager->logged_student;
    Student friend = getStudentFromSet(course_manager->students, otherId);
    if (friend == NULL) return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    if (isFriend(logged_in, friend)) return COURSE_MANAGER_ALREADY_FRIEND;
    if (isThereFriendRequest(logged_in, friend) == false) return COURSE_MANAGER_NOT_REQUESTED;
    if (strcmp(action, "accept") != 0 && strcmp(action, "reject") != 0) return COURSE_MANAGER_INVALID_PARAMETERS;

    if (strcmp(action, "accept") == 0) {
        StudentResult add_result = addFriend(logged_in, friend);
        if (add_result == STUDENT_OUT_OF_MEMORY) return COURSE_MANAGER_OUT_OF_MEMORY;
        add_result = addFriend(friend, logged_in);
        if (add_result == STUDENT_OUT_OF_MEMORY) {
            removeFriend(friend, logged_in); // bring the system to the way it was before the action
            return COURSE_MANAGER_OUT_OF_MEMORY;
        }
        // remove the logged student's request from the friend's requests list if exist
        removeFriendRequest(logged_in, friend);
    }
    // in any case (reject or accept) - remove the friend request from the logged student
    removeFriendRequest(friend, logged_in);
    return COURSE_MANAGER_OK;
}

/**
 * unFriend - remove the friendship between the logged student and the student with the given id
 * @param course_manager - the course manager that the student is logged to
 * @param otherId - the id of the student to remove the friendship with
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 * COURSE_MANAGER_NOT_FRIEND - if there is no friendship between the logged student and the student with the given id
 * (or the other id is the id of the loggen student)
 */
CourseManagerResult unFriend(CourseManager course_manager, int otherId) {
    if (course_manager == NULL || course_manager->students == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;

    Student friend = getStudentFromSet(course_manager->students, otherId);
    if (friend == NULL) return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    Student logged_in = course_manager->logged_student;
    if (isFriend(logged_in, friend) == false || studentCompare(logged_in, friend) == 0) return COURSE_MANAGER_NOT_FRIEND;
    removeFriend(logged_in, friend);
    removeFriend(friend, logged_in); // remove both ways
    return COURSE_MANAGER_OK;
}

/**
 * addGrade - adds the given grade to the logged student's grade sheet,
 * attacked to the given course in the given semester.
 * @param course_manager - the course manager that the student is logged to
 * @param semester - the semester the grade is attached to (must be positive number)
 * @param course_id - the id of the course the grade is attached to (must be positive number lower than 1000000000)
 * @param points - the number of points for the course the grade is attached to
 * (must be in the format of X or X.0 or X.5, while X is an array of digits)
 * @param grade - the grade to add to the grade sheet (must be number between 0 and 100)
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_INVALID_PARAMETERS - if one of the parameters is not valid
 */
CourseManagerResult addGrade(CourseManager course_manager, int semester, int course_id, char* points, int grade) {
    if (course_manager == NULL || course_manager->students == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;

    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    Student logged_in = course_manager->logged_student;

    StudentResult add_result = studentAddGrade(logged_in, semester, course_id, points, grade);
    if (add_result == STUDENT_OUT_OF_MEMORY || add_result == STUDENT_NULL_ARGUMENT) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (add_result == STUDENT_INVALID_PARAMETER) return COURSE_MANAGER_INVALID_PARAMETERS;
    return COURSE_MANAGER_OK;
}

/**
 * removeGrade - removes the last grade of the course with the given id in the given semester of the logged student
 * @param course_manager - the course manager that the student is logged to
 * @param semester - the semester which the grade to remove is in
 * @param course_id - the id of the course of the grade to remove
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_COURSE_DOES_NOT_EXIST - if there are no grade for the logged student in the given course at the given semester
 */
CourseManagerResult removeGrade(CourseManager course_manager, int semester, int course_id) {
    if (course_manager == NULL || course_manager->students == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    StudentResult remove_result = studentRemoveGrade(course_manager->logged_student, semester, course_id);
    if (remove_result == STUDENT_OUT_OF_MEMORY) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (remove_result == STUDENT_COURSE_DOES_NOT_EXIST) return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
    return COURSE_MANAGER_OK;
}

/**
 * updateGrade - updates the grade of the course with the given id to new_grade for the logged student.
 * updates the grade of the last semester the logged student had grade in this course.
 * @param course_manager - the course manager that the student is logged to
 * @param course_id - the id of the course of the grade to update
 * @param new_grade - the new grade for the course (must be integer between 0 and 100)
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_COURSE_DOES_NOT_EXIST - if there are no grades for the logged student in the given course
 * COURSE_MANAGER_INVALID_PARAMETERS - if the new grade is not valid
 */
CourseManagerResult updateGrade(CourseManager course_manager, int course_id, int new_grade) {
    if (course_manager == NULL || course_manager->students == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    if (new_grade < 0 || new_grade > 100) return COURSE_MANAGER_INVALID_PARAMETERS;
    StudentResult update_result = studentUpdateGrade(course_manager->logged_student, course_id, new_grade);
    if (update_result == STUDENT_COURSE_DOES_NOT_EXIST) return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
    return COURSE_MANAGER_OK;
}

/**
 * printFullReport - prints full grades report for the logged student into the given output channel
 * @param course_manager - the course manager that the student is logged to
 * @param output_channel - the channel to print the report to
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult printFullReport(CourseManager course_manager, FILE* output_channel) {
    if (course_manager == NULL || course_manager->students == NULL || output_channel == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    StudentResult print_result = studentPrintFullReport(course_manager->logged_student, output_channel);
    if (print_result == STUDENT_OUT_OF_MEMORY) return COURSE_MANAGER_OUT_OF_MEMORY;
    return COURSE_MANAGER_OK;
}

/**
 * printCleanReport - prints grades report of the logged student containing the effective grades of all the student's
 * courses, sorted by course id (and also by semester number for sport courses with the same id).
 * the print will be to the given output channel.
 * @param course_manager - the course manager that the student is logged to
 * @param output_channel - the channel to print the report to
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult printCleanReport(CourseManager course_manager, FILE* output_channel) {
    if (course_manager == NULL || course_manager->students == NULL || output_channel == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    StudentResult print_result = studentPrintCleanReport(course_manager->logged_student, output_channel);
    if (print_result == STUDENT_OUT_OF_MEMORY) return COURSE_MANAGER_OUT_OF_MEMORY;
    return COURSE_MANAGER_OK;
}

/**
 * printBestGrades - prints the best effective sheet grades of the logged student. the amount of grades printed is
 * given (must be positive number)
 * @param course_manager - the course manager that the student is logged to
 * @param amount - the number of grades to print (must be positive number)
 * @param output_channel - the channel to print the report to
 * @return
 * COURSE_MANAGER_INVALID_PARAMETERS - if the amount given is not valid
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult printBestGrades(CourseManager course_manager, int amount, FILE* output_channel) {
    if (course_manager == NULL || course_manager->students == NULL || output_channel == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    if (amount < 1) return COURSE_MANAGER_INVALID_PARAMETERS;
    StudentResult print_result = studentPrintBestOrWorstGrades(course_manager->logged_student, amount, true,
                                                               output_channel);
    if (print_result == STUDENT_OUT_OF_MEMORY) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    return COURSE_MANAGER_OK;
}

/**
 * printWorstGrades - prints the worst effective sheet grades of the logged student. the amount of grades printed is
 * given (must be positive number)
 * @param course_manager - the course manager that the student is logged to
 * @param amount - the number of grades to print (must be positive number)
 * @param output_channel - the channel to print the grades to
 * @return
 * COURSE_MANAGER_INVALID_PARAMETERS - if the amount given is not valid
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult printWorstGrades(CourseManager course_manager, int amount, FILE* output_channel) {
    if (course_manager == NULL || course_manager->students == NULL || output_channel == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    if (amount < 1) return COURSE_MANAGER_INVALID_PARAMETERS;
    StudentResult print_result = studentPrintBestOrWorstGrades(course_manager->logged_student, amount, false,
                                                               output_channel);
    if (print_result == STUDENT_OUT_OF_MEMORY) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    return COURSE_MANAGER_OK;
}

/**
 * insertGradeIntoArrayIfHigher - checks if the given grade if higher than at least one of the grades inside the array.
 * if it is - insert it to the array, sorted, and insert the student into the same index in the students array.
 * @param array - the array to insert the grade into
 * @param length - the length of both arrays
 * @param grade - the grade to insert
 */
static void insertGradeIntoArrayIfHigher(int* grades, int student_best_grade, Student** students_array,
                                         Student student, int length) {
    assert(grades != NULL && students_array != NULL && student != NULL);
    for (int i=length - 1; i >= 0; i--) {
        if (*students_array[i] == NULL) {
            grades[i] = student_best_grade;
            *students_array[i] = student;
            break;
        } else if (student_best_grade > grades[i] ||
                ((student_best_grade == grades[i]) && (studentCompare(student, *students_array[i]) == -1))) {
            for (int j = 0; j < i; j++) {
                grades[j] = grades[j + 1];
                if (*students_array[j+1] != NULL) {
                    *students_array[j] = *students_array[j+1];
                }
            }
            grades[i] = student_best_grade;
            *students_array[i] = student;
            break;
        }
    }
}

/**
 * courseManagerPrintFriendsArray - prints the names of the students in the given array, from last one to the first one.
 * stop printing if reaching NULL.
 * @param best_graded_friends - the array of students to print
 * @param length - the length of the array
 * @param output_channel - the output channel to print to
 */
static void courseManagerPrintFriendsArray(Student** best_graded_friends, int length, FILE* output_channel) {
    assert(best_graded_friends != NULL && output_channel != NULL);
    for (int i=length - 1; i >= 0; i--) {
        if (*best_graded_friends[i] == NULL) {
            break;
        }
        studentPrintName(*best_graded_friends[i], output_channel);
    }
}

/**
 * getFriendsWithBestGrades - inner function that searches the logged student's friends and returns an array in the
 * given length of the friends with the best grade in the course with the given id (sorted from best to least good, from
 * top to bottom)
 * @param course_manager - the course manager that the student is logged to
 * @param course_id - the id of the course to search friends grade
 * @param amount - the length of the array
 * @param friends_with_best_grades - pointer to the array of students to return the result in
 * @return
 * COURSE_MANAGER_OUT_OF_MEMORY - if there was a memory error
 * COURSE_OK - otherwise
 */
static CourseManagerResult getFriendsWithBestGrades(CourseManager course_manager, int course_id, int length,
                                                    Student** friends_with_best_grades) {
    int current_friend_best_grade;
    int* best_grades = (int*)malloc(sizeof(int) * length);
    if (best_grades == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;
    for (int i = 0; i < length; i++) {
        best_grades[i] = -1;
    }
    Set friends = studentGetStudentFriends(course_manager->logged_student);
    if (friends == NULL) {
        free(best_grades);
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    Student current_friend;
    SET_FOREACH(int*, current_friend_id, friends) {
        current_friend = getStudentFromSet(course_manager->students, *current_friend_id);
        current_friend_best_grade = studentGetBestGradeInCourse(current_friend, course_id);
        if (current_friend_best_grade != -1) { // -1 mean there is not grade for this student in this course
            insertGradeIntoArrayIfHigher(best_grades, current_friend_best_grade, friends_with_best_grades, current_friend,
                                         length);
        }
    }
    free(best_grades);
    setDestroy(friends);
    return COURSE_MANAGER_OK;
}

/**
 * printReferenceSources - print the names of the logged student's friends who had the best grades in the course with
 * the given course id. the amount of names printed is given (must be positive number).
 * @param course_manager  - the course manager that the student is logged to
 * @param course_id - the id of the course to search reference sources for
 * @param amount - the number of friends' names to print
 * @param output_channel - the channel to print the names to
 * @return
 * COURSE_MANAGER_INVALID_PARAMETERS - if the amount given is not valid
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult printReferenceSources(CourseManager course_manager, int course_id,int amount,FILE* output_channel) {
    if (course_manager == NULL || course_manager->students == NULL || output_channel == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    if (amount < 1) return COURSE_MANAGER_INVALID_PARAMETERS;
    Student** best_students = malloc(sizeof(Student*) * amount);
    if (best_students == NULL) return COURSE_MANAGER_OUT_OF_MEMORY;
    for (int i = 0; i < amount; i++) { // initialize array of pointer to pointers
        best_students[i] = malloc(sizeof(Student*));
        if (best_students[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(best_students[j]);
            }
            free(best_students);
            return COURSE_MANAGER_OUT_OF_MEMORY;
        }
        *best_students[i] = NULL;
    }
    CourseManagerResult get_grades_result = getFriendsWithBestGrades(course_manager, course_id, amount, best_students);
    if (get_grades_result == COURSE_MANAGER_OK) {
        courseManagerPrintFriendsArray(best_students, amount, output_channel);
    }
    for (int i = 0; i < amount; i++) {
        free(best_students[i]);
    }
    free(best_students);
    return get_grades_result;
}

/**
 * sendFacultyReqeust - send the given request to the faculty from the logged student, regarding the course with the
 * given course id, and prints the answer to the given output channel.
 * @param course_manager - the course manager that the student is logged to
 * @param course_id - the id of the course the request is about
 * @param request - the request itself (can be one of "cancel_course", "register_course" or "remove_course")
 * @param output_channel - the channel to print the response to
 * @return
 * COURSE_MANAGER_INVALID_PARAMETERS - if the request given is not valid
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_COURSE_DOES_NOT_EXIST - if the student request to remove a course he do not have
 */
CourseManagerResult sendFacultyReqeust(CourseManager course_manager, int course_id, char* request,
                                       FILE* output_channel) {
    if (course_manager == NULL || course_manager->students == NULL || output_channel == NULL) {
        return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    if (course_manager->logged_student == NULL) return COURSE_MANAGER_NOT_LOGGED_IN;
    if (strcmp(request, available_requests[2]) == 0) { // "remove_course"
        int student_best_grade = studentGetBestGradeInCourse(course_manager->logged_student, course_id);
        if (student_best_grade == -1) {
            return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
        }
    }
    if ((strcmp(request, available_requests[0]) != 0) && (strcmp(request, available_requests[1]) != 0) &&
        (strcmp(request, available_requests[2]) != 0)) {
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    mtmFacultyResponse(output_channel, faculty_response);
    return COURSE_MANAGER_OK;
}

/**
 * destroyCourseManager - deallocate all the memory the course_manager used
 * @param course_manager  - the course_manager to destroy
 */
void destroyCourseManager(CourseManager course_manager) {
    if (course_manager == NULL) return;
    setDestroy(course_manager->students);
    free(course_manager);
}