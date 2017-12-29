#include <stdlib.h>
#include "test_utilities.h"
#include "../student.h"
#include <stdio.h>

static bool testStudentCreate() {
    //StudentResult studentCreate(int id, char* firstName, char* lastName, Student *student);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    studentDestroy(student_test);

    student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, NULL, "Wershal", &student_test) == STUDENT_NULL_ARGUMENT);
    ASSERT_TEST(studentCreate(111111118, "Ariel", NULL, &student_test) == STUDENT_NULL_ARGUMENT);
    ASSERT_TEST(studentCreate(111111118, NULL, NULL, &student_test) == STUDENT_NULL_ARGUMENT);
    ASSERT_TEST(studentCreate(1111111111, "Ariel", "Wershal", &student_test) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentCreate(-111111118, "Ariel", "Wershal", &student_test) == STUDENT_INVALID_PARAMETER);

    studentDestroy(student_test);
    return true;
}

static bool testStudentCopy() {
    //SetElement studentCopy(SetElement student);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    ASSERT_TEST(addFriendRequest(student_test, student_test2) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test2) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3.5", 95) == STUDENT_OK);
    Student student_test3 = NULL;
    student_test3 = studentCopy(student_test);
    ASSERT_TEST(studentCompare(student_test, student_test3) == 0);
    Student student_test4 = NULL;
    student_test4 = studentCopy(NULL);
    ASSERT_TEST(student_test4 == NULL);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    studentDestroy(student_test4);
    return true;
}

static bool testStudentCompare() {
    //int studentCompare(SetElement student1, SetElement student2);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel2", "Wershal2", &student_test3) == STUDENT_OK);

    ASSERT_TEST(studentCompare(student_test, student_test2) == -1);
    ASSERT_TEST(studentCompare(student_test2, student_test) == 1);
    ASSERT_TEST(studentCompare(student_test, student_test3) == 0);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    return true;
}

static bool testGetStudentFromSet() {
    //Student getStudentFromSet(Set set, int id);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);

    Set students_set = setCreate(studentCopy,studentDestroy,studentCompare);
    ASSERT_TEST(setAdd(students_set, student_test) == SET_SUCCESS);
    ASSERT_TEST(setAdd(students_set, student_test2) == SET_SUCCESS);

    Student student_test3 = getStudentFromSet(students_set, 111111118);
    ASSERT_TEST(studentCompare(student_test, student_test3) == 0);
    Student student_test4 = getStudentFromSet(students_set, 222222226);
    ASSERT_TEST(studentCompare(student_test2, student_test4) == 0);
    ASSERT_TEST(getStudentFromSet(students_set, 123456789) == NULL);
    ASSERT_TEST(getStudentFromSet(NULL, 111111118) == NULL);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    setDestroy(students_set);
    return true;
}

static bool testStudentGetStudentFriends() {
    //Set studentGetStudentFriends(Student student);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(333333334, "test", "tester", &student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test2) == STUDENT_OK);

    Set friends = studentGetStudentFriends(student_test);
    int counter = 0;
    SET_FOREACH(int*, current_friend, friends) {
        if (counter == 0) {
            ASSERT_TEST(*current_friend == 222222226);
            counter ++;
        } else if (counter == 1) {
            ASSERT_TEST(*current_friend == 333333334);
            counter++;
        }
    }
    ASSERT_TEST(counter == 2);

    Set friends2 = studentGetStudentFriends(student_test2);
    counter = 0;
    SET_FOREACH(int*, current_friend, friends2) {
        counter++;
    }
    ASSERT_TEST(counter == 0);

    ASSERT_TEST(studentGetStudentFriends(NULL) == NULL);

    setDestroy(friends);
    setDestroy(friends2);
    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    return true;
}

static bool testAddFriendRequest() {
    //StudentResult addFriendRequest(Student student, Student friend);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(333333334, "test", "tester", &student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriendRequest(student_test, student_test2) == STUDENT_OK);

    ASSERT_TEST(addFriendRequest(NULL, student_test2) == STUDENT_NULL_ARGUMENT);
    ASSERT_TEST(addFriendRequest(student_test, NULL) == STUDENT_NULL_ARGUMENT);

    ASSERT_TEST(addFriend(student_test, student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriendRequest(student_test, student_test3) == STUDENT_ALREADY_FRIEND);
    ASSERT_TEST(addFriendRequest(student_test, student_test) == STUDENT_ALREADY_FRIEND);
    ASSERT_TEST(addFriendRequest(student_test, student_test2) == STUDENT_ALREADY_REQUESTED);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    return true;
}

static bool testRemoveFriendRequest() {
    //StudentResult removeFriendRequest(Student student, Student friend);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(333333334, "test", "tester", &student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriendRequest(student_test, student_test2) == STUDENT_OK);

    ASSERT_TEST(removeFriendRequest(student_test, student_test3) == STUDENT_REQUEST_NOT_EXIST);
    ASSERT_TEST(removeFriendRequest(student_test2, student_test) == STUDENT_REQUEST_NOT_EXIST);
    ASSERT_TEST(removeFriendRequest(student_test, student_test2) == STUDENT_OK);
    ASSERT_TEST(removeFriendRequest(student_test, student_test2) == STUDENT_REQUEST_NOT_EXIST);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    return true;
}

static bool testIsThereFriendRequest() {
    //bool isThereFriendRequest(Student student, Student friend);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(333333334, "test", "tester", &student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriendRequest(student_test, student_test2) == STUDENT_OK);

    ASSERT_TEST(isThereFriendRequest(student_test2, student_test) == true);
    ASSERT_TEST(isThereFriendRequest(student_test, student_test3) == false);
    ASSERT_TEST(isThereFriendRequest(NULL, student_test3) == false);
    ASSERT_TEST(isThereFriendRequest(student_test, NULL) == false);
    ASSERT_TEST(isThereFriendRequest(student_test, student_test) == true);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    return true;
}

static bool testAddFriend() {
    //StudentResult addFriend(Student student, Student friend);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(333333334, "test", "tester", &student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test2) == STUDENT_OK);

    ASSERT_TEST(addFriend(student_test, student_test2) == STUDENT_ALREADY_FRIEND);
    ASSERT_TEST(addFriend(student_test2, student_test) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test) == STUDENT_ALREADY_FRIEND);
    ASSERT_TEST(addFriend(NULL, student_test2) == STUDENT_NULL_ARGUMENT);
    ASSERT_TEST(addFriend(student_test, NULL) == STUDENT_NULL_ARGUMENT);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    return true;
}

static bool testIsFriend() {
    //bool isFriend(Student student, Student friend);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(333333334, "test", "tester", &student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test2) == STUDENT_OK);

    ASSERT_TEST(isFriend(student_test, student_test2) == true);
    ASSERT_TEST(isFriend(student_test2, student_test) == false);
    ASSERT_TEST(isFriend(student_test, student_test3) == false);
    ASSERT_TEST(isFriend(NULL, student_test2) == false);
    ASSERT_TEST(isFriend(student_test, NULL) == false);
    ASSERT_TEST(isFriend(student_test, student_test) == true);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    return true;
}

static bool testRemoveFriend() {
    //void removeFriend(Student student1, Student student2);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(333333334, "test", "tester", &student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test, student_test2) == STUDENT_OK);

    ASSERT_TEST(isFriend(student_test, student_test2) == true);
    ASSERT_TEST(isFriend(student_test, student_test3) == true);
    removeFriend(student_test, student_test2);
    ASSERT_TEST(isFriend(student_test, student_test2) == false);
    ASSERT_TEST(isFriend(student_test, student_test3) == true);
    removeFriend(student_test, student_test2);
    removeFriend(student_test, NULL);
    removeFriend(NULL, student_test2);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    return true;
}

static bool testCopyInt() {
    //SetElement copyInt(SetElement element);
    int number1 = 1;
    int* number2;
    number2 = copyInt(&number1);
    ASSERT_TEST(*number2 == 1);

    destroyInt(number2);
    return true;
}

static bool testCompareInt() {
    //int compareInt(SetElement element1, SetElement element2);
    int number1 = 1, number2 = 2, number3 = 1;
    ASSERT_TEST(compareInt(&number1, &number2) == -1);
    ASSERT_TEST(compareInt(&number2, &number1) == 1);
    ASSERT_TEST(compareInt(&number1, &number3) == 0);

    return true;
}

static bool testRemoveStudentFromFriendsSet(){
    //void removeStudentFromFriendsSet(Set set, Student student);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    Student student_test3 = NULL;
    ASSERT_TEST(studentCreate(333333334, "test", "tester", &student_test3) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test3, student_test) == STUDENT_OK);
    ASSERT_TEST(addFriendRequest(student_test, student_test2) == STUDENT_OK);
    ASSERT_TEST(addFriend(student_test2, student_test) == STUDENT_OK);

    Set students_set = setCreate(studentCopy, studentDestroy, studentCompare);
    ASSERT_TEST(setAdd(students_set, student_test) == SET_SUCCESS);
    ASSERT_TEST(setAdd(students_set, student_test2) == SET_SUCCESS);
    ASSERT_TEST(setAdd(students_set, student_test3) == SET_SUCCESS);

    ASSERT_TEST(isFriend(student_test2, student_test) == true);
    ASSERT_TEST(isFriend(student_test3, student_test) == true);
    ASSERT_TEST(isThereFriendRequest(student_test2, student_test) == true);
    removeStudentFromFriendsSet(students_set, student_test);
    int counter = 0;
    Student student_from_set1 = NULL, student_from_set2 = NULL, student_from_set3 = NULL;
    SET_FOREACH(Student, current_student, students_set) {
        if (counter == 0) {
            student_from_set1 = current_student;
        } else if (counter == 1) {
            student_from_set2 = current_student;
        } else if (counter == 2) {
            student_from_set3 = current_student;
        }
    }
    ASSERT_TEST(isFriend(student_from_set2, student_from_set1) == false);
    ASSERT_TEST(isFriend(student_from_set3, student_from_set1) == false);
    ASSERT_TEST(isThereFriendRequest(student_from_set2, student_from_set1) == false);

    studentDestroy(student_test);
    studentDestroy(student_test2);
    studentDestroy(student_test3);
    setDestroy(students_set);
    return true;
}

static bool testStudentAddGrade() {
    //StudentResult studentAddGrade(Student student, int semester, int course_id, char* points, int grade);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3.5", 95) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234118, "3", 85) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 110471, "3.5", 80) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 110471, "3.5", 95) == STUDENT_OK);

    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, NULL, 95) == STUDENT_NULL_ARGUMENT);
    ASSERT_TEST(studentAddGrade(student_test, -1, 110471, "3.5", 95) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentAddGrade(student_test, 1, -1, "3.5", 95) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentAddGrade(student_test, 1, 1000000000, "3.5", 95) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3.5", -3) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3.5", 103) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3.8", 96) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3a.5", 95) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3.55", 95) == STUDENT_INVALID_PARAMETER);

    studentDestroy(student_test);
    return true;
}

static bool testStudentRemoveGrade() {
    //StudentResult studentRemoveGrade(Student student, int semester, int course_id);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3.5", 95) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234118, "3", 85) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 110471, "3.5", 95) == STUDENT_OK);

    ASSERT_TEST(studentRemoveGrade(student_test, 1, 110471) == STUDENT_OK);
    ASSERT_TEST(studentRemoveGrade(student_test, 1, 110471) == STUDENT_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(studentRemoveGrade(student_test, 5, 110471) == STUDENT_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(studentRemoveGrade(student_test, 2, 110472) == STUDENT_COURSE_DOES_NOT_EXIST);

    studentDestroy(student_test);
    return true;
}

static bool testStudentUpdateGrade() {
    //StudentResult studentUpdateGrade(Student student, int course_id, int new_grade);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 110471, "3.5", 70) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234118, "3", 85) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 110471, "3.5", 95) == STUDENT_OK);

    ASSERT_TEST(studentGetBestGradeInCourse(student_test, 110471) == 95);
    ASSERT_TEST(studentUpdateGrade(student_test, 110471, 80) == STUDENT_OK);
    ASSERT_TEST(studentGetBestGradeInCourse(student_test, 110471) == 80);

    ASSERT_TEST(studentUpdateGrade(student_test, 110471, -2) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentUpdateGrade(student_test, 110471, 200) == STUDENT_INVALID_PARAMETER);

    ASSERT_TEST(studentUpdateGrade(student_test, 110472, 80) == STUDENT_COURSE_DOES_NOT_EXIST);

    studentDestroy(student_test);
    return true;
}

static bool testStudentPrintFullReport() {
    //StudentResult studentPrintFullReport(Student student, FILE* output_channel);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104012, "5.5", 84) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104167, "5.0", 43) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104167, "5.0", 71) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234114, "4.0", 88) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234145, "3.0", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 394808, "1.0", 92) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 104013, "5.5", 66) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 104013, "5.5", 63) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 114071, "3.5", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234118, "3.0", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234122, "3.0", 28) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234122, "3.0", 96) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234141, "3.0", 73) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 394806, "1.0", 96) == STUDENT_OK);
    printf("\n");
    studentPrintFullReport(student_test, stdout);
    //should print
    /**Grade sheet of Ariel Wershal, id 111111118
    ========================================
     Course: 104012		Points: 5.5		Grade: 84
    Course: 104167		Points: 5.0		Grade: 43
    Course: 104167		Points: 5.0		Grade: 71
    Course: 234114		Points: 4.0		Grade: 88
    Course: 234145		Points: 3.0		Grade: 98
    Course: 394808		Points: 1.0		Grade: 92
     ========================================
    Semester 1		Points: 18.5		Average: 84.05		Success: 79%

    Course: 104013		Points: 5.5		Grade: 66
    Course: 104013		Points: 5.5		Grade: 63
    Course: 114071		Points: 3.5		Grade: 98
    Course: 234118		Points: 3.0		Grade: 98
    Course: 234122		Points: 3.0		Grade: 28
    Course: 234122		Points: 3.0		Grade: 96
    Course: 234141		Points: 3.0		Grade: 73
    Course: 394806		Points: 1.0		Grade: 96
     ========================================
    Semester 2		Points: 19.0		Average: 83.50		Success: 89%

     Points: 37.5		Average: 83.77		Success: 84%
    **/

    studentPrintFullReport(NULL, stdout);
    studentPrintFullReport(student_test, NULL);

    studentDestroy(student_test);
    return true;
}

static bool testStudentPrintCleanReport() {
    //StudentResult studentPrintCleanReport(Student student, FILE* output_channel);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104012, "5.5", 84) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104167, "5.0", 43) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104167, "5.0", 71) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234114, "4.0", 88) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234145, "3.0", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 394808, "1.0", 92) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 104013, "5.5", 66) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 104013, "5.5", 63) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 114071, "3.5", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234118, "3.0", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234122, "3.0", 28) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234122, "3.0", 96) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234141, "3.0", 73) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 394806, "1.0", 96) == STUDENT_OK);
    printf("\n");
    studentPrintCleanReport(student_test, stdout);
    //should print
    /**
    Grade sheet of Ariel Wershal, id 111111118
    ========================================
    Course: 104012		Points: 5.5		Grade: 84
    Course: 104013		Points: 5.5		Grade: 63
    Course: 104167		Points: 5.0		Grade: 71
    Course: 114071		Points: 3.5		Grade: 98
    Course: 234114		Points: 4.0		Grade: 88
    Course: 234118		Points: 3.0		Grade: 98
    Course: 234122		Points: 3.0		Grade: 96
    Course: 234141		Points: 3.0		Grade: 73
    Course: 234145		Points: 3.0		Grade: 98
    Course: 394806		Points: 1.0		Grade: 96
    Course: 394808		Points: 1.0		Grade: 92
    ========================================
    Points: 37.5		Average: 83.77
    **/

    studentPrintCleanReport(NULL, stdout);
    studentPrintCleanReport(student_test, NULL);

    studentDestroy(student_test);
    return true;
}

static bool testStudentPrintBestOrWorstGrades() {
    //StudentResult studentPrintBestOrWorstGrades(Student student, int amount, bool best, FILE* output_channel);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104012, "5.5", 84) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104167, "5.0", 43) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104167, "5.0", 71) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234114, "4.0", 88) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 234145, "3.0", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 394808, "1.0", 92) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 104013, "5.5", 66) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 104013, "5.5", 63) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 114071, "3.5", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234118, "3.0", 98) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234122, "3.0", 28) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234122, "3.0", 96) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 234141, "3.0", 73) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 394806, "1.0", 96) == STUDENT_OK);

    printf("\n");
    ASSERT_TEST(studentPrintBestOrWorstGrades(student_test, 5, true, stdout) == STUDENT_OK);
    //should print
    /**
    Course: 234145		Points: 3.0		Grade: 98
    Course: 114071		Points: 3.5		Grade: 98
    Course: 234118		Points: 3.0		Grade: 98
    Course: 234122		Points: 3.0		Grade: 96
    Course: 394806		Points: 1.0		Grade: 96
    **/

    ASSERT_TEST(studentPrintBestOrWorstGrades(student_test, -2, true, stdout) == STUDENT_INVALID_PARAMETER);
    ASSERT_TEST(studentPrintBestOrWorstGrades(NULL, 5, true, stdout) == STUDENT_OUT_OF_MEMORY);
    ASSERT_TEST(studentPrintBestOrWorstGrades(student_test, 5, true, NULL) == STUDENT_OUT_OF_MEMORY);

    Student student_test2 = NULL;
    ASSERT_TEST(studentCreate(222222226, "Eran", "Channover", &student_test2) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test2, 1, 104012, "5.5", 84) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test2, 1, 104167, "5.0", 43) == STUDENT_OK);
    printf("\n");
    ASSERT_TEST(studentPrintBestOrWorstGrades(student_test2, 5, true, stdout) == STUDENT_OK);
    //should print
    /**
    Course: 104012		Points: 5.5		Grade: 84
    Course: 104167		Points: 5.0		Grade: 43
    **/

    printf("\n");
    ASSERT_TEST(studentPrintBestOrWorstGrades(student_test, 3, false, stdout) == STUDENT_OK);
    //should print
    /**
    Course: 104013		Points: 5.5		Grade: 63
    Course: 104167		Points: 5.0		Grade: 71
    Course: 234141		Points: 3.0		Grade: 73
    **/

    printf("\n");
    ASSERT_TEST(studentPrintBestOrWorstGrades(student_test2, 5, false, stdout) == STUDENT_OK);
    //should print
    /**
    Course: 104167		Points: 5.0		Grade: 43
    Course: 104012		Points: 5.5		Grade: 84
    **/

    studentDestroy(student_test);
    studentDestroy(student_test2);
    return true;
}

static bool testStudentGetBestGradeInCourse() {
    //int studentGetBestGradeInCourse(Student student, int course_id);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104012, "5.5", 84) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104167, "5.0", 80) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 1, 104167, "5.0", 71) == STUDENT_OK);
    ASSERT_TEST(studentAddGrade(student_test, 2, 104167, "5.0", 76) == STUDENT_OK);

    ASSERT_TEST(studentGetBestGradeInCourse(student_test, 104167) == 80);
    ASSERT_TEST(studentGetBestGradeInCourse(student_test, 104111) == -1);
    ASSERT_TEST(studentGetBestGradeInCourse(NULL, 104111) == -1);

    studentDestroy(student_test);
    return true;
}

static bool testStudentPrintName() {
    //void studentPrintName(Student student, FILE* output_channel);
    Student student_test = NULL;
    ASSERT_TEST(studentCreate(111111118, "Ariel", "Wershal", &student_test) == STUDENT_OK);

    printf("\n");
    studentPrintName(student_test, stdout);
    //should print
    //Ariel Wershal

    studentPrintName(NULL, stdout);
    studentPrintName(student_test, NULL);

    studentDestroy(student_test);
    return true;
}

int runAllStudentTests() {
    RUN_TEST(testStudentCreate);
    RUN_TEST(testStudentCopy);
    RUN_TEST(testStudentCompare);
    RUN_TEST(testGetStudentFromSet);
    RUN_TEST(testStudentGetStudentFriends);
    RUN_TEST(testAddFriendRequest);
    RUN_TEST(testRemoveFriendRequest);
    RUN_TEST(testIsThereFriendRequest);
    RUN_TEST(testAddFriend);
    RUN_TEST(testRemoveFriend);
    RUN_TEST(testIsFriend);
    RUN_TEST(testCopyInt);
    RUN_TEST(testCompareInt);
    RUN_TEST(testRemoveStudentFromFriendsSet);
    RUN_TEST(testStudentAddGrade);
    RUN_TEST(testStudentRemoveGrade);
    RUN_TEST(testStudentUpdateGrade);
    RUN_TEST(testStudentPrintFullReport);
    RUN_TEST(testStudentPrintCleanReport);
    RUN_TEST(testStudentPrintBestOrWorstGrades);
    RUN_TEST(testStudentGetBestGradeInCourse);
    RUN_TEST(testStudentPrintName);

    return 0;
}