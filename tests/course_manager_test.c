#include <stdlib.h>
#include "test_utilities.h"
#include "../course_manager.h"
#include <stdio.h>

static bool testCourseManagerCreate() {
    //CourseManager courseManagerCreate();
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testAddStudent() {
    //MtmErrorCode addStudent(CourseManager  course_manager, int id, char* firstName, char* lastName);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 222222226, "Eran", "Channover") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel","Wershal") == COURSE_MANAGER_STUDENT_ALREADY_EXISTS);
    ASSERT_TEST(addStudent(course_manager_test, -111111118, "Ariel", "Wershal") == COURSE_MANAGER_INVALID_PARAMETERS);
    ASSERT_TEST(addStudent(course_manager_test, 1111111111, "Ariel", "Wershal") == COURSE_MANAGER_INVALID_PARAMETERS);
    ASSERT_TEST(addStudent(course_manager_test, 222222226, NULL, "Channover") == COURSE_MANAGER_OUT_OF_MEMORY);
    ASSERT_TEST(addStudent(course_manager_test, 222222226, "Eran", NULL) == COURSE_MANAGER_OUT_OF_MEMORY);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testRemoveStudent() {
    //CourseManagerResult removeStudent(CourseManager course_manager, int id);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 222222226, "Eran", "Channover") == COURSE_MANAGER_OK);
    ASSERT_TEST(removeStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(removeStudent(course_manager_test, 111111118) == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testLogInStudent() {
    //CourseManagerResult logInStudent(CourseManager course_manager, int id);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 222222226, "Eran", "Channover") == COURSE_MANAGER_OK);
    ASSERT_TEST(logInStudent(course_manager_test, 111111111) == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST);
    ASSERT_TEST(logInStudent(course_manager_test, -2) == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_ALREADY_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 222222226) == COURSE_MANAGER_ALREADY_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 222222222) == COURSE_MANAGER_ALREADY_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, -2) == COURSE_MANAGER_ALREADY_LOGGED_IN);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testLogOutStudent() {
    //CourseManagerResult logOutStudent(CourseManager course_manager);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(logOutStudent(course_manager_test) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(logOutStudent(course_manager_test) == COURSE_MANAGER_OK);
    ASSERT_TEST(logOutStudent(course_manager_test) == COURSE_MANAGER_NOT_LOGGED_IN);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testSendFriendRequest() {
    //CourseManagerResult sendFriendRequest(CourseManager course_manager, int id);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 222222226, "Eran", "Channover") == COURSE_MANAGER_OK);
    ASSERT_TEST(sendFriendRequest(course_manager_test, 222222226) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(sendFriendRequest(course_manager_test, 2) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(sendFriendRequest(course_manager_test, 222222226) == COURSE_MANAGER_OK);
    ASSERT_TEST(sendFriendRequest(course_manager_test, 222222222) == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST);
    ASSERT_TEST(sendFriendRequest(course_manager_test, 111111118) == COURSE_MANAGER_ALREADY_FRIEND);
    ASSERT_TEST(sendFriendRequest(course_manager_test, 222222226) == COURSE_MANAGER_ALREADY_REQUESTED);
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 222222226);
    handleFriendRequest(course_manager_test, 111111118, "accept");
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 111111118);
    ASSERT_TEST(sendFriendRequest(course_manager_test, 222222226) == COURSE_MANAGER_ALREADY_FRIEND);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testHandleFriendRequest () {
    //CourseManagerResult handleFriendRequest(CourseManager course_manager, int otherId, char* action);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 222222226, "Eran", "Channover") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 333333334, "test", "tester") == COURSE_MANAGER_OK);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 222222226, "accept") == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 2, "test") == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 2, "accept") == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 2, "test") == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 222222226, "accept") == COURSE_MANAGER_NOT_REQUESTED);
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 222222226);
    sendFriendRequest(course_manager_test, 111111118);
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 333333334);
    sendFriendRequest(course_manager_test, 111111118);
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 111111118);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 222222226, "accept") == COURSE_MANAGER_OK);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 222222226, "accept") == COURSE_MANAGER_ALREADY_FRIEND);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 111111118, "accept") == COURSE_MANAGER_ALREADY_FRIEND);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 111111118, "reject") == COURSE_MANAGER_ALREADY_FRIEND);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 333333334, "test") == COURSE_MANAGER_INVALID_PARAMETERS);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 333333334, "reject") == COURSE_MANAGER_OK);
    ASSERT_TEST(handleFriendRequest(course_manager_test, 333333334, "reject") == COURSE_MANAGER_NOT_REQUESTED);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testUnFriend() {
    //CourseManagerResult unFriend(CourseManager course_manager, int otherId);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 222222226, "Eran", "Channover") == COURSE_MANAGER_OK);
    ASSERT_TEST(addStudent(course_manager_test, 333333334, "test", "tester") == COURSE_MANAGER_OK);
    ASSERT_TEST(unFriend(course_manager_test, 222222226) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(unFriend(course_manager_test, 2) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(unFriend(course_manager_test, 2) == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST);
    ASSERT_TEST(unFriend(course_manager_test, 222222226) == COURSE_MANAGER_NOT_FRIEND);
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 222222226);
    sendFriendRequest(course_manager_test, 111111118);
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 333333334);
    sendFriendRequest(course_manager_test, 111111118);
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 111111118);
    handleFriendRequest(course_manager_test, 222222226, "accept");
    ASSERT_TEST(unFriend(course_manager_test, 333333334) == COURSE_MANAGER_NOT_FRIEND);
    ASSERT_TEST(unFriend(course_manager_test, 222222226) == COURSE_MANAGER_OK);
    ASSERT_TEST(unFriend(course_manager_test, 222222226) == COURSE_MANAGER_NOT_FRIEND);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testAddGrade() {
    //CourseManagerResult addGrade(CourseManager course_manager, int semester, int course_id, char* points, int grade);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 1, 110471, "3.5", 95) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(addGrade(course_manager_test, 1, 110471, "3.5", 105) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(addGrade(course_manager_test, 1, 110471, "3.5", -2) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(addGrade(course_manager_test, 1, 110471, "3a.5", 95) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(addGrade(course_manager_test, 1, 110471, "3.8", 95) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(addGrade(course_manager_test, -1, 110471, "3.5", 95) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(addGrade(course_manager_test, 1, -2, "3.5", 95) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(addGrade(course_manager_test, 1, 1111111111, "3.5", 95) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 1, 110471, "3.5", 95) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 1, 234118, "3", 85) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 2, 234118, "3", 90) == COURSE_MANAGER_OK);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testRemoveGrade() {
    //CourseManagerResult removeGrade(CourseManager course_manager, int semester, int course_id);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(removeGrade(course_manager_test, 1, 110471) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(removeGrade(course_manager_test, -1, 110471) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(removeGrade(course_manager_test, 1, -110471) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(removeGrade(course_manager_test, 1, 1111111111) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 1, 110471, "3.5", 95) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 1, 234118, "3", 85) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 2, 234118, "3", 90) == COURSE_MANAGER_OK);
    ASSERT_TEST(removeGrade(course_manager_test, 1, 11111) == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(removeGrade(course_manager_test, -1, 110471) == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(removeGrade(course_manager_test, 1, -110471) == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(removeGrade(course_manager_test, 1, 1111111111) == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(removeGrade(course_manager_test, 2, 110471) == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(removeGrade(course_manager_test, 2, 234118) == COURSE_MANAGER_OK);
    ASSERT_TEST(removeGrade(course_manager_test, 2, 234118) == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testUpdateGrade() {
    //CourseManagerResult updateGrade(CourseManager course_manager, int course_id, int new_grade);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(updateGrade(course_manager_test,110471, 100) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(updateGrade(course_manager_test, 110471, 105) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(updateGrade(course_manager_test, 110471, -2) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(updateGrade(course_manager_test, -110471, 90) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(updateGrade(course_manager_test, 1111111111, 90) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 1, 110471, "3.5", 95) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 1, 234118, "3", 70) == COURSE_MANAGER_OK);
    ASSERT_TEST(addGrade(course_manager_test, 2, 234118, "3", 75) == COURSE_MANAGER_OK);
    ASSERT_TEST(updateGrade(course_manager_test, 11111, 90) == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(updateGrade(course_manager_test, 1111111111, 90) == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);
    ASSERT_TEST(updateGrade(course_manager_test, 110471, 105) == COURSE_MANAGER_INVALID_PARAMETERS);
    ASSERT_TEST(updateGrade(course_manager_test, 110471, -5) == COURSE_MANAGER_INVALID_PARAMETERS);
    ASSERT_TEST(updateGrade(course_manager_test, 234118, 85) == COURSE_MANAGER_OK);
    printf("\n");
    printBestGrades(course_manager_test, 3, stdout);
    // should print (just to check it is updated)
    // Course: 110471		Points: 3.5		Grade: 95
    // Course: 234118		Points: 3.0		Grade: 85

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testPrintFullReport() {
    //CourseManagerResult printFullReport(CourseManager course_manager, FILE* output_channel);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(printFullReport(course_manager_test, stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    addGrade(course_manager_test, 1, 104012, "5.5", 84);
    addGrade(course_manager_test, 1, 104167, "5.0", 43);
    addGrade(course_manager_test, 1, 104167, "5.0", 71);
    addGrade(course_manager_test, 1, 234114, "4.0", 88);
    addGrade(course_manager_test, 1, 234145, "3.0", 98);
    addGrade(course_manager_test, 1, 394808, "1.0", 92);
    addGrade(course_manager_test, 2, 104013, "5.5", 66);
    addGrade(course_manager_test, 2, 104013, "5.5", 63);
    addGrade(course_manager_test, 2, 114071, "3.5", 98);
    addGrade(course_manager_test, 2, 234118, "3.0", 98);
    addGrade(course_manager_test, 2, 234122, "3.0", 28);
    addGrade(course_manager_test, 2, 234122, "3.0", 96);
    addGrade(course_manager_test, 2, 234141, "3.0", 73);
    addGrade(course_manager_test, 2, 394806, "1.0", 96);
    addGrade(course_manager_test, 3, 94412, "4.0", 80);
    addGrade(course_manager_test, 3, 104134, "2.5", 82);
    addGrade(course_manager_test, 3, 134058, "3.0", 70);
    addGrade(course_manager_test, 3, 234218, "3.0", 75);
    addGrade(course_manager_test, 3, 234262, "3.0", 94);
    addGrade(course_manager_test, 3, 234293, "4.0", 71);
    addGrade(course_manager_test, 3, 394902, "1.5", 96);
    addGrade(course_manager_test, 4, 134020, "3.5", 56);
    addGrade(course_manager_test, 4, 134020, "3.5", 76);
    addGrade(course_manager_test, 4, 234123, "4.5", 94);
    addGrade(course_manager_test, 4, 234247, "3.0", 95);
    addGrade(course_manager_test, 4, 234267, "3.0", 60);
    addGrade(course_manager_test, 4, 234267, "3.0", 68);
    addGrade(course_manager_test, 4, 236353, "3.0", 78);
    addGrade(course_manager_test, 4, 394902, "1.5", 92);
    addGrade(course_manager_test, 5, 234107, "4.0", 51);
    addGrade(course_manager_test, 5, 234107, "4.0", 63);
    addGrade(course_manager_test, 5, 234900, "2.0", 74);
    addGrade(course_manager_test, 5, 236327, "3.0", 77);
    addGrade(course_manager_test, 5, 236334, "3.0", 64);
    addGrade(course_manager_test, 5, 236334, "3.0", 68);
    addGrade(course_manager_test, 5, 236343, "3.0", 80);
    addGrade(course_manager_test, 5, 236360, "3.0", 71);
    addGrade(course_manager_test, 5, 324440, "2.0", 79);
    addGrade(course_manager_test, 5, 394902, "1.5", 97);
    addGrade(course_manager_test, 6, 94591, "3.5", 86);
    addGrade(course_manager_test, 6, 234319, "3.0", 93);
    addGrade(course_manager_test, 6, 236370, "3.0", 90);
    addGrade(course_manager_test, 6, 236504, "3.0", 89);
    addGrade(course_manager_test, 6, 236604, "2.0", 65);
    addGrade(course_manager_test, 6, 236653, "3.0", 87);
    addGrade(course_manager_test, 6, 324290, "2.0", 73);
    addGrade(course_manager_test, 6, 394902, "1.5", 93);
    addGrade(course_manager_test, 7, 104174, "3.5", 25);
    addGrade(course_manager_test, 7, 104174, "3.5", 90);
    addGrade(course_manager_test, 7, 236350, "3.0", 84);
    addGrade(course_manager_test, 7, 236503, "3.0", 71);
    addGrade(course_manager_test, 7, 236603, "2.0", 71);
    addGrade(course_manager_test, 7, 236703, "3.0", 100);
    addGrade(course_manager_test, 7, 394902, "1.5", 98);
    printf("\n");
    ASSERT_TEST(printFullReport(course_manager_test, stdout) == COURSE_MANAGER_OK);
    //should print
    /**
     Grade sheet of Ariel Wershal, id 111111118
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

    Course: 094412		Points: 4.0		Grade: 80
    Course: 104134		Points: 2.5		Grade: 82
    Course: 134058		Points: 3.0		Grade: 70
    Course: 234218		Points: 3.0		Grade: 75
    Course: 234262		Points: 3.0		Grade: 94
    Course: 234293		Points: 4.0		Grade: 71
    Course: 394902		Points: 1.5		Grade: 96
    ========================================
    Semester 3		Points: 21.0		Average: 79.52		Success: 100%

    Course: 134020		Points: 3.5		Grade: 56
    Course: 134020		Points: 3.5		Grade: 76
    Course: 234123		Points: 4.5		Grade: 94
    Course: 234247		Points: 3.0		Grade: 95
    Course: 234267		Points: 3.0		Grade: 60
    Course: 234267		Points: 3.0		Grade: 68
    Course: 236353		Points: 3.0		Grade: 78
    Course: 394902		Points: 1.5		Grade: 92
    ========================================
    Semester 4		Points: 18.5		Average: 83.78		Success: 100%

    Course: 234107		Points: 4.0		Grade: 51
    Course: 234107		Points: 4.0		Grade: 63
    Course: 234900		Points: 2.0		Grade: 74
    Course: 236327		Points: 3.0		Grade: 77
    Course: 236334		Points: 3.0		Grade: 64
    Course: 236334		Points: 3.0		Grade: 68
    Course: 236343		Points: 3.0		Grade: 80
    Course: 236360		Points: 3.0		Grade: 71
    Course: 324440		Points: 2.0		Grade: 79
    Course: 394902		Points: 1.5		Grade: 97
    ========================================
    Semester 5		Points: 21.5		Average: 74.02		Success: 86%

    Course: 094591		Points: 3.5		Grade: 86
    Course: 234319		Points: 3.0		Grade: 93
    Course: 236370		Points: 3.0		Grade: 90
    Course: 236504		Points: 3.0		Grade: 89
    Course: 236604		Points: 2.0		Grade: 65
    Course: 236653		Points: 3.0		Grade: 87
    Course: 324290		Points: 2.0		Grade: 73
    Course: 394902		Points: 1.5		Grade: 93
    ========================================
    Semester 6		Points: 21.0		Average: 85.40		Success: 100%

    Course: 104174		Points: 3.5		Grade: 25
    Course: 104174		Points: 3.5		Grade: 90
    Course: 236350		Points: 3.0		Grade: 84
    Course: 236503		Points: 3.0		Grade: 71
    Course: 236603		Points: 2.0		Grade: 71
    Course: 236703		Points: 3.0		Grade: 100 << Wow!
    Course: 394902		Points: 1.5		Grade: 98
    ========================================
    Semester 7		Points: 16.0		Average: 85.56		Success: 82%

    Points: 135.5		Average: 82.03		Success: 91%
     */

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testPrintCleanReport() {
    //CourseManagerResult printCleanReport(CourseManager course_manager, FILE* output_channel);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(printCleanReport(course_manager_test, stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    addGrade(course_manager_test, 1, 104012, "5.5", 84);
    addGrade(course_manager_test, 1, 104167, "5.0", 43);
    addGrade(course_manager_test, 1, 104167, "5.0", 71);
    addGrade(course_manager_test, 1, 234114, "4.0", 88);
    addGrade(course_manager_test, 1, 234145, "3.0", 98);
    addGrade(course_manager_test, 1, 394808, "1.0", 92);
    addGrade(course_manager_test, 2, 104013, "5.5", 66);
    addGrade(course_manager_test, 2, 104013, "5.5", 63);
    addGrade(course_manager_test, 2, 114071, "3.5", 98);
    addGrade(course_manager_test, 2, 234118, "3.0", 98);
    addGrade(course_manager_test, 2, 234122, "3.0", 28);
    addGrade(course_manager_test, 2, 234122, "3.0", 96);
    addGrade(course_manager_test, 2, 234141, "3.0", 73);
    addGrade(course_manager_test, 2, 394806, "1.0", 96);
    addGrade(course_manager_test, 3, 94412, "4.0", 80);
    addGrade(course_manager_test, 3, 104134, "2.5", 82);
    addGrade(course_manager_test, 3, 134058, "3.0", 70);
    addGrade(course_manager_test, 3, 234218, "3.0", 75);
    addGrade(course_manager_test, 3, 234262, "3.0", 94);
    addGrade(course_manager_test, 3, 234293, "4.0", 71);
    addGrade(course_manager_test, 3, 394902, "1.5", 96);
    addGrade(course_manager_test, 4, 134020, "3.5", 56);
    addGrade(course_manager_test, 4, 134020, "3.5", 76);
    addGrade(course_manager_test, 4, 234123, "4.5", 94);
    addGrade(course_manager_test, 4, 234247, "3.0", 95);
    addGrade(course_manager_test, 4, 234267, "3.0", 60);
    addGrade(course_manager_test, 4, 234267, "3.0", 68);
    addGrade(course_manager_test, 4, 236353, "3.0", 78);
    addGrade(course_manager_test, 4, 394902, "1.5", 92);
    addGrade(course_manager_test, 5, 234107, "4.0", 51);
    addGrade(course_manager_test, 5, 234107, "4.0", 63);
    addGrade(course_manager_test, 5, 234900, "2.0", 74);
    addGrade(course_manager_test, 5, 236327, "3.0", 77);
    addGrade(course_manager_test, 5, 236334, "3.0", 64);
    addGrade(course_manager_test, 5, 236334, "3.0", 68);
    addGrade(course_manager_test, 5, 236343, "3.0", 80);
    addGrade(course_manager_test, 5, 236360, "3.0", 71);
    addGrade(course_manager_test, 5, 324440, "2.0", 79);
    addGrade(course_manager_test, 5, 394902, "1.5", 97);
    addGrade(course_manager_test, 6, 94591, "3.5", 86);
    addGrade(course_manager_test, 6, 234319, "3.0", 93);
    addGrade(course_manager_test, 6, 236370, "3.0", 90);
    addGrade(course_manager_test, 6, 236504, "3.0", 89);
    addGrade(course_manager_test, 6, 236604, "2.0", 65);
    addGrade(course_manager_test, 6, 236653, "3.0", 87);
    addGrade(course_manager_test, 6, 324290, "2.0", 73);
    addGrade(course_manager_test, 6, 394902, "1.5", 93);
    addGrade(course_manager_test, 7, 104174, "3.5", 25);
    addGrade(course_manager_test, 7, 104174, "3.5", 90);
    addGrade(course_manager_test, 7, 236350, "3.0", 84);
    addGrade(course_manager_test, 7, 236503, "3.0", 71);
    addGrade(course_manager_test, 7, 236603, "2.0", 71);
    addGrade(course_manager_test, 7, 236703, "3.0", 100);
    addGrade(course_manager_test, 7, 394902, "1.5", 98);
    printf("\n");
    ASSERT_TEST(printCleanReport(course_manager_test, stdout) == COURSE_MANAGER_OK);
    //should print
    /**
     Grade sheet of Ariel Wershal, id 111111118
    ========================================
    Course: 094412		Points: 4.0		Grade: 80
    Course: 094591		Points: 3.5		Grade: 86
    Course: 104012		Points: 5.5		Grade: 84
    Course: 104013		Points: 5.5		Grade: 63
    Course: 104134		Points: 2.5		Grade: 82
    Course: 104167		Points: 5.0		Grade: 71
    Course: 104174		Points: 3.5		Grade: 90
    Course: 114071		Points: 3.5		Grade: 98
    Course: 134020		Points: 3.5		Grade: 76
    Course: 134058		Points: 3.0		Grade: 70
    Course: 234107		Points: 4.0		Grade: 63
    Course: 234114		Points: 4.0		Grade: 88
    Course: 234118		Points: 3.0		Grade: 98
    Course: 234122		Points: 3.0		Grade: 96
    Course: 234123		Points: 4.5		Grade: 94
    Course: 234141		Points: 3.0		Grade: 73
    Course: 234145		Points: 3.0		Grade: 98
    Course: 234218		Points: 3.0		Grade: 75
    Course: 234247		Points: 3.0		Grade: 95
    Course: 234262		Points: 3.0		Grade: 94
    Course: 234267		Points: 3.0		Grade: 68
    Course: 234293		Points: 4.0		Grade: 71
    Course: 234319		Points: 3.0		Grade: 93
    Course: 234900		Points: 2.0		Grade: 74
    Course: 236327		Points: 3.0		Grade: 77
    Course: 236334		Points: 3.0		Grade: 68
    Course: 236343		Points: 3.0		Grade: 80
    Course: 236350		Points: 3.0		Grade: 84
    Course: 236353		Points: 3.0		Grade: 78
    Course: 236360		Points: 3.0		Grade: 71
    Course: 236370		Points: 3.0		Grade: 90
    Course: 236503		Points: 3.0		Grade: 71
    Course: 236504		Points: 3.0		Grade: 89
    Course: 236603		Points: 2.0		Grade: 71
    Course: 236604		Points: 2.0		Grade: 65
    Course: 236653		Points: 3.0		Grade: 87
    Course: 236703		Points: 3.0		Grade: 100 << Wow!
    Course: 324290		Points: 2.0		Grade: 73
    Course: 324440		Points: 2.0		Grade: 79
    Course: 394806		Points: 1.0		Grade: 96
    Course: 394808		Points: 1.0		Grade: 92
    Course: 394902		Points: 1.5		Grade: 96
    Course: 394902		Points: 1.5		Grade: 92
    Course: 394902		Points: 1.5		Grade: 97
    Course: 394902		Points: 1.5		Grade: 93
    Course: 394902		Points: 1.5		Grade: 98
    ========================================
    Points: 135.5		Average: 82.03
     */

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testPrintBestGrades() {
    //CourseManagerResult printBestGrades(CourseManager course_manager, int amount, FILE* output_channel);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(printBestGrades(course_manager_test, 3, stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(printBestGrades(course_manager_test, -3, stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    addGrade(course_manager_test, 1, 104012, "5.5", 84);
    addGrade(course_manager_test, 1, 104167, "5.0", 43);
    addGrade(course_manager_test, 1, 104167, "5.0", 71);
    addGrade(course_manager_test, 1, 234114, "4.0", 88);
    addGrade(course_manager_test, 1, 234145, "3.0", 98);
    addGrade(course_manager_test, 1, 394808, "1.0", 92);
    addGrade(course_manager_test, 2, 104013, "5.5", 66);
    addGrade(course_manager_test, 2, 104013, "5.5", 63);
    addGrade(course_manager_test, 2, 114071, "3.5", 98);
    addGrade(course_manager_test, 2, 234118, "3.0", 98);
    addGrade(course_manager_test, 2, 234122, "3.0", 28);
    addGrade(course_manager_test, 2, 234122, "3.0", 96);
    addGrade(course_manager_test, 2, 234141, "3.0", 73);
    addGrade(course_manager_test, 2, 394806, "1.0", 96);
    addGrade(course_manager_test, 3, 94412, "4.0", 80);
    addGrade(course_manager_test, 3, 104134, "2.5", 82);
    addGrade(course_manager_test, 3, 134058, "3.0", 70);
    addGrade(course_manager_test, 3, 234218, "3.0", 75);
    addGrade(course_manager_test, 3, 234262, "3.0", 94);
    addGrade(course_manager_test, 3, 234293, "4.0", 71);
    addGrade(course_manager_test, 3, 394902, "1.5", 96);
    addGrade(course_manager_test, 4, 134020, "3.5", 56);
    addGrade(course_manager_test, 4, 134020, "3.5", 76);
    addGrade(course_manager_test, 4, 234123, "4.5", 94);
    addGrade(course_manager_test, 4, 234247, "3.0", 95);
    addGrade(course_manager_test, 4, 234267, "3.0", 60);
    addGrade(course_manager_test, 4, 234267, "3.0", 68);
    addGrade(course_manager_test, 4, 236353, "3.0", 78);
    addGrade(course_manager_test, 4, 394902, "1.5", 92);
    addGrade(course_manager_test, 5, 234107, "4.0", 51);
    addGrade(course_manager_test, 5, 234107, "4.0", 63);
    addGrade(course_manager_test, 5, 234900, "2.0", 74);
    addGrade(course_manager_test, 5, 236327, "3.0", 77);
    addGrade(course_manager_test, 5, 236334, "3.0", 64);
    addGrade(course_manager_test, 5, 236334, "3.0", 68);
    addGrade(course_manager_test, 5, 236343, "3.0", 80);
    addGrade(course_manager_test, 5, 236360, "3.0", 71);
    addGrade(course_manager_test, 5, 324440, "2.0", 79);
    addGrade(course_manager_test, 5, 394902, "1.5", 97);
    addGrade(course_manager_test, 6, 94591, "3.5", 86);
    addGrade(course_manager_test, 6, 234319, "3.0", 93);
    addGrade(course_manager_test, 6, 236370, "3.0", 90);
    addGrade(course_manager_test, 6, 236504, "3.0", 89);
    addGrade(course_manager_test, 6, 236604, "2.0", 65);
    addGrade(course_manager_test, 6, 236653, "3.0", 87);
    addGrade(course_manager_test, 6, 324290, "2.0", 73);
    addGrade(course_manager_test, 6, 394902, "1.5", 93);
    addGrade(course_manager_test, 7, 104174, "3.5", 25);
    addGrade(course_manager_test, 7, 104174, "3.5", 90);
    addGrade(course_manager_test, 7, 236350, "3.0", 84);
    addGrade(course_manager_test, 7, 236503, "3.0", 71);
    addGrade(course_manager_test, 7, 236603, "2.0", 71);
    addGrade(course_manager_test, 7, 236703, "3.0", 100);
    addGrade(course_manager_test, 7, 394902, "1.5", 98);
    printf("\n");
    ASSERT_TEST(printBestGrades(course_manager_test, -3, stdout) == COURSE_MANAGER_INVALID_PARAMETERS);
    ASSERT_TEST(printBestGrades(course_manager_test, 3, stdout) == COURSE_MANAGER_OK);
    //should print
    /**
     Course: 236703		Points: 3.0		Grade: 100 << Wow!
    Course: 234145		Points: 3.0		Grade: 98
    Course: 114071		Points: 3.5		Grade: 98
     */

    addStudent(course_manager_test, 222222226, "Eran", "Channover");
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 222222226);
    ASSERT_TEST(printBestGrades(course_manager_test, 3, stdout) == COURSE_MANAGER_OK);
    // should print nothing
    addGrade(course_manager_test, 1, 104701, "3.5", 100);
    printf("\n");
    ASSERT_TEST(printBestGrades(course_manager_test, 3, stdout) == COURSE_MANAGER_OK);
    // should print
    //Course: 104701		Points: 3.5		Grade: 100 << Wow!

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testPrintWorstGrades() {
    //CourseManagerResult printWorstGrades(CourseManager course_manager, int amount, FILE* output_channel);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(printWorstGrades(course_manager_test, 3, stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(printWorstGrades(course_manager_test, -3, stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    addGrade(course_manager_test, 1, 104012, "5.5", 84);
    addGrade(course_manager_test, 1, 104167, "5.0", 43);
    addGrade(course_manager_test, 1, 104167, "5.0", 71);
    addGrade(course_manager_test, 1, 234114, "4.0", 88);
    addGrade(course_manager_test, 1, 234145, "3.0", 98);
    addGrade(course_manager_test, 1, 394808, "1.0", 92);
    addGrade(course_manager_test, 2, 104013, "5.5", 66);
    addGrade(course_manager_test, 2, 104013, "5.5", 63);
    addGrade(course_manager_test, 2, 114071, "3.5", 98);
    addGrade(course_manager_test, 2, 234118, "3.0", 98);
    addGrade(course_manager_test, 2, 234122, "3.0", 28);
    addGrade(course_manager_test, 2, 234122, "3.0", 96);
    addGrade(course_manager_test, 2, 234141, "3.0", 73);
    addGrade(course_manager_test, 2, 394806, "1.0", 96);
    addGrade(course_manager_test, 3, 94412, "4.0", 80);
    addGrade(course_manager_test, 3, 104134, "2.5", 82);
    addGrade(course_manager_test, 3, 134058, "3.0", 70);
    addGrade(course_manager_test, 3, 234218, "3.0", 75);
    addGrade(course_manager_test, 3, 234262, "3.0", 94);
    addGrade(course_manager_test, 3, 234293, "4.0", 71);
    addGrade(course_manager_test, 3, 394902, "1.5", 96);
    addGrade(course_manager_test, 4, 134020, "3.5", 56);
    addGrade(course_manager_test, 4, 134020, "3.5", 76);
    addGrade(course_manager_test, 4, 234123, "4.5", 94);
    addGrade(course_manager_test, 4, 234247, "3.0", 95);
    addGrade(course_manager_test, 4, 234267, "3.0", 60);
    addGrade(course_manager_test, 4, 234267, "3.0", 68);
    addGrade(course_manager_test, 4, 236353, "3.0", 78);
    addGrade(course_manager_test, 4, 394902, "1.5", 92);
    addGrade(course_manager_test, 5, 234107, "4.0", 51);
    addGrade(course_manager_test, 5, 234107, "4.0", 63);
    addGrade(course_manager_test, 5, 234900, "2.0", 74);
    addGrade(course_manager_test, 5, 236327, "3.0", 77);
    addGrade(course_manager_test, 5, 236334, "3.0", 64);
    addGrade(course_manager_test, 5, 236334, "3.0", 68);
    addGrade(course_manager_test, 5, 236343, "3.0", 80);
    addGrade(course_manager_test, 5, 236360, "3.0", 71);
    addGrade(course_manager_test, 5, 324440, "2.0", 79);
    addGrade(course_manager_test, 5, 394902, "1.5", 97);
    addGrade(course_manager_test, 6, 94591, "3.5", 86);
    addGrade(course_manager_test, 6, 234319, "3.0", 93);
    addGrade(course_manager_test, 6, 236370, "3.0", 90);
    addGrade(course_manager_test, 6, 236504, "3.0", 89);
    addGrade(course_manager_test, 6, 236604, "2.0", 65);
    addGrade(course_manager_test, 6, 236653, "3.0", 87);
    addGrade(course_manager_test, 6, 324290, "2.0", 73);
    addGrade(course_manager_test, 6, 394902, "1.5", 93);
    addGrade(course_manager_test, 7, 104174, "3.5", 25);
    addGrade(course_manager_test, 7, 104174, "3.5", 90);
    addGrade(course_manager_test, 7, 236350, "3.0", 84);
    addGrade(course_manager_test, 7, 236503, "3.0", 71);
    addGrade(course_manager_test, 7, 236603, "2.0", 71);
    addGrade(course_manager_test, 7, 236703, "3.0", 100);
    addGrade(course_manager_test, 7, 394902, "1.5", 98);
    printf("\n");
    ASSERT_TEST(printWorstGrades(course_manager_test, -3, stdout) == COURSE_MANAGER_INVALID_PARAMETERS);
    ASSERT_TEST(printWorstGrades(course_manager_test, 3, stdout) == COURSE_MANAGER_OK);
    //should print
    /**
    Course: 104013		Points: 5.5		Grade: 63
    Course: 234107		Points: 4.0		Grade: 63
    Course: 236604		Points: 2.0		Grade: 65
     */

    addStudent(course_manager_test, 222222226, "Eran", "Channover");
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 222222226);
    ASSERT_TEST(printWorstGrades(course_manager_test, 3, stdout) == COURSE_MANAGER_OK);
    // should print nothing
    addGrade(course_manager_test, 1, 104701, "3.5", 100);
    printf("\n");
    ASSERT_TEST(printWorstGrades(course_manager_test, 3, stdout) == COURSE_MANAGER_OK);
    // should print
    //Course: 104701		Points: 3.5		Grade: 100 << Wow!

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testPrintReferenceSources() {
    //CourseManagerResult printReferenceSources(CourseManager course_manager, int course_id, int amount,
    // FILE* output_channel);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(printReferenceSources(course_manager_test, 104701, 3, stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(printReferenceSources(course_manager_test, 104701, -3, stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    addStudent(course_manager_test, 222222226, "Eran", "Channover");
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 222222226);
    addGrade(course_manager_test, 1, 104701, "3.5", 90);
    sendFriendRequest(course_manager_test, 111111118);
    addStudent(course_manager_test, 333333334, "Test", "Tester");
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 333333334);
    addGrade(course_manager_test, 1, 104701, "3.5", 100);
    sendFriendRequest(course_manager_test, 111111118);
    logOutStudent(course_manager_test);
    addStudent(course_manager_test, 444444445, "Test2", "Tester2");
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 444444445);
    addGrade(course_manager_test, 1, 104701, "3.5", 100);
    addGrade(course_manager_test, 1, 104701, "3.5", 60);
    sendFriendRequest(course_manager_test, 111111118);
    logOutStudent(course_manager_test);
    logInStudent(course_manager_test, 111111118);
    ASSERT_TEST(printReferenceSources(course_manager_test, 104701, 3, stdout) == COURSE_MANAGER_OK);
    // should print nothing (has no friends)
    handleFriendRequest(course_manager_test, 222222226, "accept");
    handleFriendRequest(course_manager_test, 333333334, "accept");
    handleFriendRequest(course_manager_test, 444444445, "accept");
    ASSERT_TEST(printReferenceSources(course_manager_test, 104701, -3, stdout) == COURSE_MANAGER_INVALID_PARAMETERS);
    printf("\n");
    ASSERT_TEST(printReferenceSources(course_manager_test, 104701, 3, stdout) == COURSE_MANAGER_OK);
    // should print
    /**
     Test Tester
     Test2 Tester2
     Eran Channover
     */
    ASSERT_TEST(printReferenceSources(course_manager_test, 234118, 3, stdout) == COURSE_MANAGER_OK);
    // should print nothing
    printf("\n");
    ASSERT_TEST(printReferenceSources(course_manager_test, 104701, 1, stdout) == COURSE_MANAGER_OK);
    // should print
    /**
     Test Tester
     */

    destroyCourseManager(course_manager_test);
    return true;
}

static bool testSendFacultyReqeust() {
    //CourseManagerResult sendFacultyReqeust(CourseManager course_manager, int course_id, char* request,
    // FILE* output_channel);
    CourseManager course_manager_test = courseManagerCreate();
    ASSERT_TEST(course_manager_test != NULL);
    ASSERT_TEST(addStudent(course_manager_test, 111111118, "Ariel", "Wershal") == COURSE_MANAGER_OK);
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 104701,"cancel_course",stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 104701, "test", stdout) == COURSE_MANAGER_NOT_LOGGED_IN);
    ASSERT_TEST(logInStudent(course_manager_test, 111111118) == COURSE_MANAGER_OK);
    addGrade(course_manager_test, 1, 104701, "3.5", 84);
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 104701, "test", stdout) == COURSE_MANAGER_INVALID_PARAMETERS);
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 234118, "remove_course", stdout)
                == COURSE_MANAGER_COURSE_DOES_NOT_EXIST);
    printf("\n");
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 234118, "cancel_course", stdout) == COURSE_MANAGER_OK);
    // should print
    // Faculty response: your request was rejected
    printf("\n");
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 234118, "register_course", stdout) == COURSE_MANAGER_OK);
    // should print
    // Faculty response: your request was rejected
    printf("\n");
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 107401, "cancel_course", stdout) == COURSE_MANAGER_OK);
    // should print
    // Faculty response: your request was rejected
    printf("\n");
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 104701, "register_course", stdout) == COURSE_MANAGER_OK);
    // should print
    // Faculty response: your request was rejected
    printf("\n");
    ASSERT_TEST(sendFacultyReqeust(course_manager_test, 104701, "remove_course", stdout) == COURSE_MANAGER_OK);
    // should print
    // Faculty response: your request was rejected

    destroyCourseManager(course_manager_test);
    return true;
}


int main() {
    RUN_TEST(testCourseManagerCreate);
    RUN_TEST(testAddStudent);
    RUN_TEST(testRemoveStudent);
    RUN_TEST(testLogInStudent);
    RUN_TEST(testLogOutStudent);
    RUN_TEST(testSendFriendRequest);
    RUN_TEST(testHandleFriendRequest);
    RUN_TEST(testUnFriend);
    RUN_TEST(testAddGrade);
    RUN_TEST(testRemoveGrade);
    RUN_TEST(testUpdateGrade);
    RUN_TEST(testPrintFullReport);
    RUN_TEST(testPrintCleanReport);
    RUN_TEST(testPrintBestGrades);
    RUN_TEST(testPrintWorstGrades);
    RUN_TEST(testPrintReferenceSources);
    RUN_TEST(testSendFacultyReqeust);
    
    return 0;
}