#ifndef EX3_STUDENT_H
#define EX3_STUDENT_H

#include "set.h"
#include "mtm_ex3.h"

typedef struct student_t *Student;

/** Type used for returning error codes from functions */
typedef enum StudentResult_t {
    STUDENT_OK,
    STUDENT_NULL_ARGUMENT,
    STUDENT_OUT_OF_MEMORY,
    STUDENT_ALREADY_FRIEND,
    STUDENT_ALREADY_REQUESTED,
    STUDENT_REQUEST_NOT_EXIST,
    STUDENT_INVALID_PARAMETER,
    STUDENT_COURSE_DOES_NOT_EXIST
} StudentResult;

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
StudentResult studentCreate(int id, char* firstName, char* lastName, Student *student);

/**
 * studentCopy: Creates a copy of target student.
 *
 * @param student - Target student.
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A Student with the same data as given student otherwise.
 */
Student studentCopy(Student student);

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
int studentCompare(Student student1, Student student2);

/**
 * getStudentFromSet - return the student object of the student with given id from given set.
 * @param set - the set to search the student in
 * @param id - the id of the student to search
 * @return
 * the student with the given id (if found).
 * if the student is NULL, returns NULL
 */
Student getStudentFromSet(Set set, int id);

/**
 * studentGetStudentFriends - return set of ids of the student's friends.
 * @param student - the to search for his friends
 * @return
 * set of ids of the student's friends.
 * if the student does not found, returns NULL
 */
Set studentGetStudentFriends(Student student);

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
StudentResult addFriendRequest(Student student, Student friend);

/**
 * removeFriendRequest - remove a friend request from student to friend
 * @param student - the student who sent the request
 * @param friend - the student who got the request
 * @return
 * STUDENT_REQUEST_NOT_EXIST - if there was no friend request from student to friend
 * STUDENT_OK - otherwise (success)
 */
StudentResult removeFriendRequest(Student student, Student friend);

/**
 * isThereFriendRequest - checks if there is a pending friend request from student to friend
 * @param student - the student who sent the request
 * @param friend - the student who got the request
 * @return whether there is such request or not (or they are the same student)
 */
bool isThereFriendRequest(Student student, Student friend);

/**
 * addFriend - add student2 to student1 friends list
 *
 * @param student - the student to add the friend to it's list
 * @param friend - the student to be added as friend
 * @return
 * STUDENT_NULL_ARGUMENT - if one of the argument is Null
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 * STUDENT_ALREADY_FRIEND - if friend if already in student's friend list (or they are the same student)
 * STUDENT_OK - otherwise
 */
StudentResult addFriend(Student student, Student friend);

/**
 * isFriend - checks if friend is in the friends list of student
 * @param student - the student to check it's friend list
 * @param friend - the student to check if he is in the friends list
 * @return
 * true if friend is in student's friends list (or they are the same student), false otherwise
 */
bool isFriend(Student student, Student friend);

/**
 * removeFriend - remove student1 from student2's friends list and vise versa.
 * @param student1 - the first student
 * @param student2 - the second student
 */
void removeFriend(Student student1, Student student2);

///////////////////////////////////////////////////////////////////
// functions to use for Set of ints (students' ids)
///////////////////////////////////////////////////////////////////
/**
 * copyInt - copies int
 * @param element - int pointer to copy
 * @return copied int. NULL if memory error
 */
SetElement copyInt(SetElement element);
/**
 * compareInt - compares between 2 ints
 * @param element1 - pointer to int1 to compare
 * @param element2 - pointer to int2 to compare
 * @return
 * 0 if the ints are equal.
 * 1 if the left int is bigger
 * -1 if the right int is bigger
 */
int compareInt(SetElement element1, SetElement element2);
/**
 * destroyInt - free the pointer to the int
 * @param element - pointer to int
 */
void destroyInt(SetElement element);
////////////////////////////////////////////////////////////////////

/**
 * removeStudentFromFriendsSet - remove the student details from all other students' friends details, and from other
 * student's friends requests
 * @param set - set of students
 * @param student - the student to remove
 */
void removeStudentFromFriendsSet(Set set, Student student);

/**
 * studentAddGrade - adds the given grade to the student's grade sheet, attacked to the given course
 * in the given semester.
 * @param student - the student to add the grade to
 * @param semester - the semester the grade is attached to (must be positive number)
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
StudentResult studentAddGrade(Student student, int semester, int course_id, char* points, int grade);

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
StudentResult studentRemoveGrade(Student student, int semester, int course_id);

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
StudentResult studentUpdateGrade(Student student, int course_id, int new_grade);

/**
 * studentPrintFullReport - prints full grades report of the student into the given outpt channel
 * @param student - the student to print his report
 * @param output_channel - the channel to print the report to
 * @return
 * STUDENT_OUT_OF_MEMORY - if there was a memory error
 */
StudentResult studentPrintFullReport(Student student, FILE* output_channel);

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
StudentResult studentPrintCleanReport(Student student, FILE* output_channel);

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
StudentResult studentPrintBestOrWorstGrades(Student student, int amount, bool best, FILE* output_channel);

/**
 * studentGetBestGradeInCourse - search for the best grade in all semesters in the course with the given id that the
 * given student have. if the student is NULL or do not have any grade for the course with the given id - return -1.
 * @param student - the student to search his best grade
 * @param course_id - the id of the course to search
 * @return
 * the best grade of the given student in the given course. -1 if not found.
 */
int studentGetBestGradeInCourse(Student student, int course_id);

/**
 * studentPrintName - prints the name of the student
 * @param student - the student to print his name
 * @param output_channel - the channel to print the report to
 */
void studentPrintName(Student student, FILE* output_channel);

/**
 * studentDestroy - deallocate all the data of the student
 *
 * @param student - the student object to deallocate.
 * If student is NULL nothing will be done
 */
void studentDestroy(Student student);
#endif //EX3_STUDENT_H
