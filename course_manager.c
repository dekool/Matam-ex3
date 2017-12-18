#include "course_manager.h"
#include <stdlib.h>
#include "string.h"

typedef struct course_manager_t
{
Set students;
Student logged_student;
};

/**
 * courseManagerCreate - creates a new empty course manager
 * @return
 * NULL - if allocations failed.
 * 	A new CourseManager in case of success.
 */
CourseManager courseManagerCreate() {
    CourseManager course_manager = (CourseManager) malloc(sizeof(*course_manager));
    if (course_manager == NULL) return NULL;
    course_manager->students = setCreate((SetElement)studentCopy,(SetElement)studentDestroy,(SetElement)studentCompare);
    if (course_manager->students == NULL) {
        free(course_manager);
        return NULL;
    }
    course_manager->logged_student = NULL;
    return course_manager;
}

/**
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
MtmErrorCode addStudent(CourseManager courseManager, int id, char* firstName, char* lastName) {
    if (firstName == NULL || lastName == NULL || courseManager == NULL || courseManager->students == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    Student student = NULL;
    StudentResult createResult = studentCreate(id, firstName, lastName, &student);
    if (createResult == STUDENT_OUT_OF_MEMORY) return MTM_OUT_OF_MEMORY;
    if (createResult == STUDENT_INVALID_PARAMETER) {
        studentDestroy(student);
        return MTM_INVALID_PARAMETERS;
    }
    SetResult add_result = setAdd(courseManager->students, (SetElement)student);
    studentDestroy(student);
    switch(add_result) {
        case SET_ITEM_ALREADY_EXISTS:
            return MTM_STUDENT_ALREADY_EXISTS;
        case SET_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case SET_NULL_ARGUMENT:
            return MTM_OUT_OF_MEMORY;
        default:
            break;
    }
}

/**
 * removeStudent - removes the student with the given id from the system
 * @param courseManager - the course manager to remove the student from
 * @param id  - the id of the student to be removed
 * @return
 * MTM_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 */
MtmErrorCode removeStudent(CourseManager  courseManager, int id) {
    if (courseManager == NULL || courseManager->students == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    Student student = getStudentFromSet(courseManager->students, id);
    if (student == NULL) {
        return MTM_STUDENT_DOES_NOT_EXIST;
    }
    removeStudentFromFriendsSet(courseManager->students, student);
    if (courseManager->logged_student != NULL && studentCompare(courseManager->logged_student, student) == 0) {
        logOutStudent(courseManager);
    }
    SetResult removeResult = setRemove(courseManager->students, student);
    if (removeResult == SET_ITEM_DOES_NOT_EXIST) return MTM_STUDENT_DOES_NOT_EXIST;
}

/**
 * logInStudent - log in student with the given id
 * @param courseManager - the course manager that the student is logging in to
 * @param id - the id of the student who log in
 * @return
 * MTM_ALREADY_LOGGED_IN - if a student is already logged in to the system
 * MTM_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 */
MtmErrorCode logInStudent(CourseManager courseManager, int id) {
    if (courseManager == NULL || courseManager->students == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    if (courseManager->logged_student != NULL) return MTM_ALREADY_LOGGED_IN;
    Student student = getStudentFromSet(courseManager->students, id);
    if (student == NULL) {
        return MTM_STUDENT_DOES_NOT_EXIST;
    }
    courseManager->logged_student = student;
}

/**
 * logOutStudent - log out the logged student
 * @param courseManager - the course manager that the student is logging in to
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 */
MtmErrorCode logOutStudent(CourseManager courseManager) {
    if (courseManager == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    if (courseManager->logged_student == NULL) {
        return MTM_NOT_LOGGED_IN;
    }
    courseManager->logged_student = NULL;
}

/**
 * sendFriendRequest - send friend request from the logged student to the student with the given id
 * @param courseManager - the course manager that the student is logging in to
 * @param id - the id of the student to send friend request to
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 * MTM_STUDENT_DOES_NOT_EXIST - if student with given id does not exist in the system
 * MTM_ALREADY_FRIEND - if the student with the requested id is already a friend of the logged student
 * (or the requested id is the id of the logged student)
 * MTM_ALREADY_REQUESTED - if their is already a pending friend request from the logged student to the
 * student with the given id
 */
MtmErrorCode sendFriendRequest(CourseManager courseManager, int id) {
    if (courseManager == NULL || courseManager->students == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    if (courseManager->logged_student == NULL) {
        return MTM_NOT_LOGGED_IN;
    }
    Student friend = getStudentFromSet(courseManager->students, id);
    if (friend == NULL) {
        return MTM_STUDENT_DOES_NOT_EXIST;
    }
    StudentResult add_result = addFriendRequest(courseManager->logged_student, friend);
    switch (add_result) {
        case STUDENT_ALREADY_FRIEND: return MTM_ALREADY_FRIEND;
        case STUDENT_ALREADY_REQUESTED: return MTM_ALREADY_REQUESTED;
    }
}

/**
 * handleFriendRequest - accept or reject a friend request of the logged student
 * @param courseManager - the course manager that the student is logging in to
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
MtmErrorCode handleFriendRequest(CourseManager courseManager, int otherId, char* action) {
    if (courseManager == NULL || courseManager->students == NULL) return MTM_OUT_OF_MEMORY;
    if (strcmp(action, "accept") != 0 && strcmp(action, "reject") != 0) return MTM_INVALID_PARAMETERS;
    if (courseManager->logged_student == NULL) return MTM_NOT_LOGGED_IN;

    Student logged_in = courseManager->logged_student;
    Student friend = getStudentFromSet(courseManager->students, otherId);
    if (friend == NULL) return MTM_STUDENT_DOES_NOT_EXIST;
    if (isFriend(logged_in, friend)) return MTM_ALREADY_FRIEND;
    if (isThereFriendRequest(logged_in, friend) == false) return MTM_NOT_REQUESTED;

    if (strcmp(action, "accept") == 0) {
        StudentResult add_result = addFriend(logged_in, friend);
        if (add_result == STUDENT_OUT_OF_MEMORY) return MTM_OUT_OF_MEMORY;
        add_result = addFriend(friend, logged_in);
        if (add_result == STUDENT_OUT_OF_MEMORY) {
            removeFriend(friend, logged_in); // bring the system to the way it was before the action
            return MTM_OUT_OF_MEMORY;
        }
        // remove the logged student's request from the friend's requests list if exist
        removeFriendRequest(friend, logged_in);
    }
    // in any case (reject or accept) - remove the friend request from the logged student
    removeFriendRequest(logged_in, friend);
}

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
MtmErrorCode unFriend(CourseManager courseManager, int otherId) {
    if (courseManager == NULL || courseManager->students == NULL) return MTM_OUT_OF_MEMORY;
    if (courseManager->logged_student == NULL) return MTM_NOT_LOGGED_IN;

    Student friend = getStudentFromSet(courseManager->students, otherId);
    if (friend == NULL) return MTM_STUDENT_DOES_NOT_EXIST;
    Student logged_in = courseManager->logged_student;
    if (isFriend(logged_in, friend) == false || studentCompare(logged_in, friend) == 0) return MTM_NOT_FRIEND;
    removeFriend(logged_in, friend);
    removeFriend(friend, logged_in); // remove both ways
}

/**
 * addGrade - adds the given grade to the logged student's grade sheet,
 * attacked to the given course in the given semester.
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
MtmErrorCode addGrade(CourseManager courseManager, int semester, int course_id, char* points, int grade) {
    if (courseManager == NULL || courseManager->students == NULL) return MTM_OUT_OF_MEMORY;
    if (points == NULL) return MTM_INVALID_PARAMETERS;
    if (courseManager->logged_student == NULL) return MTM_NOT_LOGGED_IN;
    Student logged_in = courseManager->logged_student;
    StudentResult add_result = studentAddGrade(logged_in, semester, course_id, points, grade);
    if (add_result == STUDENT_OUT_OF_MEMORY || add_result == STUDENT_NULL_ARGUMENT) return MTM_OUT_OF_MEMORY;
    if (add_result == STUDENT_INVALID_PARAMETER) return MTM_INVALID_PARAMETERS;
}

/**
 * removeGrade - removes the last grade of the course with the given id in the given semester of the logged student
 * @param courseManager - the course manager that the student is logged to
 * @param semester - the semester which the grade to remove is in
 * @param course_id - the id of the course of the grade to remove
 * @return
 * MTM_NOT_LOGGED_IN - if no student is logged in to the system
 * MTM_COURSE_DOES_NOT_EXIST - if there are no grade for the logged student in the given course at the given semester
 */
MtmErrorCode removeGrade(CourseManager courseManager, int semester, int course_id) {
    if (courseManager == NULL || courseManager->students == NULL) return MTM_OUT_OF_MEMORY;
    if (courseManager->logged_student == NULL) return MTM_NOT_LOGGED_IN;
    StudentResult remove_result = studentRemoveGrade(courseManager->logged_student, semester, course_id);
    if (remove_result == STUDENT_OUT_OF_MEMORY) return MTM_OUT_OF_MEMORY;
    if (remove_result == STUDENT_COURSE_DOES_NOT_EXIST) return MTM_COURSE_DOES_NOT_EXIST;
}

/**
 * destroyCourseManager - deallocate all the memory the courseManager used
 * @param courseManager  - the courseManager to destroy
 */
void destroyCourseManager(CourseManager courseManager) {
    if (courseManager == NULL) return;
    setDestroy(courseManager->students);
    free(courseManager);
}