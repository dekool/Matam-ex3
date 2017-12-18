#include "student.h"
#include <string.h>
#include <stdlib.h>
#include "assert.h"
#include "semester.h"

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
 * getStudentFromSet - return the student pointer of the student with given id from given set.
 * @param set - the set to search the student in
 * @param id - the id of the student to search
 * @return
 * pointer to the student with the given id (if found).
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