#ifndef EX3_COURSE_MANAGER_H
#define EX3_COURSE_MANAGER_H

#include "list.h"
#include "set.h"
#include "mtm_ex3.h"
#include "student.h"

typedef struct course_manager_t *CourseManager;

/**
 * courseManagerCreate - creates a new empty course manager
 * @return
 * NULL - if allocations failed.
 * 	A new CourseManager in case of success.
 */
CourseManager courseManagerCreate();

/**
 * addStudent - adds new student to the system
 * @param courseManager - the course manager to add the student to
 * @param id - the id number of the student
 * @param firstName - the student's first name
 * @param lastName - the student's last name
 * @return
 * MTM_STUDENT_ALREADY_EXISTS - if the a student with the same id is already registered in the system
 * MTM_INVALID_PARAMETERS - if the id entered is invalid (number is higher than 999999999 or negative)
 * MTM_OUT_OF_MEMORY - if there was a memory error
 */
MtmErrorCode addStudent(CourseManager  courseManager, int id, char* firstName, char* lastName);

/**
 * removeStudent - removes the student with the given id from the system
 * @param courseManager - the course manager to remove the student from
 * @param id  - the id of the student to be removed
 * @return
 * MTM_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 */
MtmErrorCode removeStudent(CourseManager courseManager, int id);

/**
 * logInStudent - log in student with the given id
 * @param courseManager - the course manager that the student is logging in to
 * @param id - the id of the student who log in
 * @return
 * MTM_ALREADY_LOGGED_IN - if a student is already logged in to the system
 * MTM_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 */
MtmErrorCode logInStudent(CourseManager courseManager, int id);

/**
 * logOutStudent - log out the logged student
 * @param courseManager - the course manager that the student is logging out from
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 */
MtmErrorCode logOutStudent(CourseManager courseManager);

/**
 * sendFriendRequest - send friend request from the logged student to the student with the given id
 * @param courseManager - the course manager that the student is logged to
 * @param id - the id of the student to send friend request to
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 * MTM_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 * MTM_ALREADY_FRIEND - if the student with the requested id is already a friend of the logged student
 * (or the requested id is the id of the logged student)
 * MTM_ALREADY_REQUESTED - if their is already a pending friend request from the logged student to the
 * student with the given id
 */
MtmErrorCode sendFriendRequest(CourseManager courseManager, int id);

/**
 * handleFriendRequest - accept or reject a friend request of the logged student
 * @param courseManager - the course manager that the student is logged to
 * @param otherId - the id of the student to handle it's request
 * @param action - accept/reject
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 * MTM_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 * MTM_ALREADY_FRIEND - if the student with the requested id is already a friend of the logged student
 * (or the requested id is the id of the logged student)
 * MTM_NOT_REQUESTED - if there is no pending friend request from the logged student to the
 * student with the given id
 * MTM_INVALID_PARAMETERS - if the action is not one of "accept" or "reject"
 */
MtmErrorCode handleFriendRequest(CourseManager courseManager, int otherId, char* action);

/**
 * unFriend - remove the friendship between the logged student and the student with the given id
 * @param courseManager - the course manager that the student is logged to
 * @param otherId - the id of the student to remove the friendship with
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 * MTM_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 * MTM_NOT_FRIEND - if there is no friendship between the logged student and the student with the given id
 * (or the other id is the id of the loggen student)
 */
MtmErrorCode unFriend(CourseManager courseManager, int otherId);

/**
 * addGrade - adds the given grade to the logged student's grade sheet,
 * attached to the given course in the given semester.
 * @param courseManager - the course manager that the student is logged to
 * @param semester - the semester the grade is attached to (must be positive number)
 * @param course_id - the id of the course the grade is attached to (must be positive number lower than 1000000000)
 * @param points - the number of points for the course the grade is attached to
 * (must be in the format of X or X.0 or X.5, while X is an array of digits)
 * @param grade - the grade to add to the grade sheet (must be number between 0 and 100)
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 * MTM_INVALID_PARAMETERS - if one of the parameters is not valid
 */
MtmErrorCode addGrade(CourseManager courseManager, int semester, int course_id, char* points, int grade);

/**
 * removeGrade - removes the last grade of the course with the given id in the given semester of the logged student
 * @param courseManager - the course manager that the student is logged to
 * @param semester - the semester which the grade to remove is in
 * @param course_id - the id of the course of the grade to remove
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 * MTM_COURSE_DOES_NOT_EXIST - if there are no grade for the logged student in the given course at the given semester
 */
MtmErrorCode removeGrade(CourseManager courseManager, int semester, int course_id);

/**
 * destroyCourseManager - deallocate all the memory the courseManager used
 * @param courseManager  - the courseManager to destroy
 */
void destroyCourseManager(CourseManager courseManager);

#endif //EX3_COURSE_MANAGER_H
