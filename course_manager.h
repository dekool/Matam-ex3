#ifndef EX3_COURSE_MANAGER_H
#define EX3_COURSE_MANAGER_H

#include "list.h"
#include "set.h"
#include "mtm_ex3.h"
#include "student.h"

typedef struct course_manager_t *CourseManager;

/** Type used for returning error codes from functions */
typedef enum CourseManagerResult_t {
    COURSE_MANAGER_OK,
    COURSE_MANAGER_OUT_OF_MEMORY, // You should exit program after this error
    COURSE_MANAGER_NOT_LOGGED_IN,
    COURSE_MANAGER_ALREADY_LOGGED_IN,
    COURSE_MANAGER_STUDENT_DOES_NOT_EXIST,
    COURSE_MANAGER_STUDENT_ALREADY_EXISTS,
    COURSE_MANAGER_NOT_FRIEND,
    COURSE_MANAGER_ALREADY_FRIEND,
    COURSE_MANAGER_NOT_REQUESTED,
    COURSE_MANAGER_ALREADY_REQUESTED,
    COURSE_MANAGER_COURSE_DOES_NOT_EXIST,
    COURSE_MANAGER_INVALID_PARAMETERS
} CourseManagerResult;

/**
 * courseManagerCreate - creates a new empty course manager
 * @return
 * NULL - if allocations failed.
 * 	A new CourseManager in case of success.
 */
CourseManager courseManagerCreate();

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
CourseManagerResult addStudent(CourseManager  course_manager, int id, char* firstName, char* lastName);

/**
 * removeStudent - removes the student with the given id from the system
 * @param course_manager - the course manager to remove the student from
 * @param id  - the id of the student to be removed
 * @return
 * COURSE_MANAGER_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 */
CourseManagerResult removeStudent(CourseManager course_manager, int id);

/**
 * logInStudent - log in student with the given id
 * @param course_manager - the course manager that the student is logging in to
 * @param id - the id of the student who log in
 * @return
 * COURSE_MANAGER_ALREADY_LOGGED_IN - if a student is already logged in to the system
 * COURSE_MANAGER_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 */
CourseManagerResult logInStudent(CourseManager course_manager, int id);

/**
 * logOutStudent - log out the logged student
 * @param course_manager - the course manager that the student is logging out from
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult logOutStudent(CourseManager course_manager);

/**
 * sendFriendRequest - send friend request from the logged student to the student with the given id
 * @param course_manager - the course manager that the student is logged to
 * @param id - the id of the student to send friend request to
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 * COURSE_MANAGER_ALREADY_FRIEND - if the student with the requested id is already a friend of the logged student
 * (or the requested id is the id of the logged student)
 * COURSE_MANAGER_ALREADY_REQUESTED - if their is already a pending friend request from the logged student to the
 * student with the given id
 */
CourseManagerResult sendFriendRequest(CourseManager course_manager, int id);

/**
 * handleFriendRequest - accept or reject a friend request of the logged student
 * @param course_manager - the course manager that the student is logged to
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
CourseManagerResult handleFriendRequest(CourseManager course_manager, int otherId, char* action);

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
CourseManagerResult unFriend(CourseManager course_manager, int otherId);

/**
 * addGrade - adds the given grade to the logged student's grade sheet,
 * attached to the given course in the given semester.
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
CourseManagerResult addGrade(CourseManager course_manager, int semester, int course_id, char* points, int grade);

/**
 * removeGrade - removes the last grade of the course with the given id in the given semester of the logged student
 * @param course_manager - the course manager that the student is logged to
 * @param semester - the semester which the grade to remove is in
 * @param course_id - the id of the course of the grade to remove
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 * COURSE_MANAGER_COURSE_DOES_NOT_EXIST - if there are no grade for the logged student in the given course at the given semester
 */
CourseManagerResult removeGrade(CourseManager course_manager, int semester, int course_id);

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
CourseManagerResult updateGrade(CourseManager course_manager, int course_id, int new_grade);

/**
 * printFullReport - prints full grades report for the logged student into the given output channel
 * @param course_manager - the course manager that the student is logged to
 * @param output_channel - the channel to print the report to
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult printFullReport(CourseManager course_manager, FILE* output_channel);

/**
 * printCleanReport - prints grades report of the logged student containing the effective grades of all the student's
 * courses, sorted by course id (and also by semester number for sport courses with the same id).
 * the print will be to the given output channel.
 * @param course_manager - the course manager that the student is logged to
 * @param output_channel - the channel to print the report to
 * @return
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult printCleanReport(CourseManager course_manager, FILE* output_channel);

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
CourseManagerResult printBestGrades(CourseManager course_manager, int amount, FILE* output_channel);

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
CourseManagerResult printWorstGrades(CourseManager course_manager, int amount, FILE* output_channel);

/**
 * printReferenceSources - print the names of the logged student's friends who had the best grades in the course with
 * the given course id. the amount of names printed is given (must be positive number).
 * @param course_manager  - the course manager that the student is logged to
 * @param course_id - the id of the course to search reference sources for
 * @param amount - the number of friends' names to print (must be positive number)
 * @param output_channel - the channel to print the names to
 * @return
 * COURSE_MANAGER_INVALID_PARAMETERS - if the amount given is not valid
 * COURSE_MANAGER_NOT_LOGGED_IN - if no student is logged in to the system
 */
CourseManagerResult printReferenceSources(CourseManager course_manager, int course_id, int amount,FILE* output_channel);

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
CourseManagerResult sendFacultyReqeust(CourseManager course_manager, int course_id, char* request,FILE* output_channel);

/**
 * destroyCourseManager - deallocate all the memory the course_manager used
 * @param course_manager  - the course_manager to destroy
 */
void destroyCourseManager(CourseManager course_manager);

#endif //EX3_COURSE_MANAGER_H
