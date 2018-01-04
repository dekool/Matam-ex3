#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "mtm_ex3.h"
#include "course_manager.h"

// Constants
#define INPUT_FLAG "-i"
#define OUTPUT_FLAG "-o"
#define ERROR_CHANNEL stderr
#define FILE_READ_MODE "r"
#define FILE_WRITE_MODE "w"
#define COMMENT_PREFIX '#'

/**
 * This type defines all possible results for a parser function
 */
typedef enum {
    PARSER_OK,
    PARSER_FAILED
} ParserResult;

/**
 * this type is just a new name for a file descriptor
 */
typedef FILE* file;

/**
 * Check if the user passed command line parameters in a valid format
 * The format of the parameters is valid if the following conditions are met:
 * - there is an odd number of parameters in total (the program name + pairs of "-flag description")
 * - the total number of parameters is at most 5 (program name + 0/2/4 IO parameters)
 * - all the parameters (except for the program name) are in the format "-flag description"
 * - the only flags used (if used) are "-i" and "-o"
 *
 * @param argc - number of command line parameters passed
 * @param argv - array of the parameters
 * @return
 * false - the passed parameters are in an invalid format
 * true - the passed parameters are in a valid format
 */
static bool validateCommandLineParameters(int argc, char** argv) {
    assert(argv != NULL);
    if(argc % 2 == 0) return false;
    if(argc > 5) return false;

    for(int i = 1; i < argc; i+=2) {
        if(strcmp(argv[i], INPUT_FLAG) && strcmp(argv[i], OUTPUT_FLAG)) return false;   // check for an invalid flag
    }
    return true;
}

/**
 * Return the description of a passed flag in the command line arguments
 *
 * @param argc - the number of command line arguments
 * @param argv - an array of the arguments
 * @param flag - the flag whose description will be returned
 * @return
 * NULL - if there is an even number of arguments (including the program name) or there is no flag as the one requested
 * the flag description in any other case
 */
static char* getFlagDescription(int argc, char** argv, char* flag) {
    assert(argv != NULL && flag != NULL);
    if(argc % 2 == 0) return NULL;
    for(int i = 1; i < argc; i+=2) {
        if(!strcmp(argv[i], flag)) return argv[i+1];
    }
    return NULL;
}

/**
 * Get the input stream that the user asks for (stdin or from file)
 *
 * @param argc - number of command line arguments
 * @param argv - array of the arguments
 * @return
 * stdin - if the user didn't specify a file to take the input from
 * a file descriptor - if the user specified a valid file name to take input from
 * NULL - if the user specified an invalid file or an error occured
 */
static file getInputStream(int argc, char** argv) {
    assert(argv != NULL);
    char* filename = getFlagDescription(argc, argv, INPUT_FLAG);
    if(filename == NULL) return stdin;  // no file specified
    file input_file = fopen(filename, FILE_READ_MODE);
    if(input_file == NULL) return NULL;
    return input_file;
}

/**
 * Get the output stream that the user asks for (stdout or into file)
 *
 * @param argc - number of command line arguments
 * @param argv - array of the arguments
 * @return
 * stdout - if the user didn't specify a file to put the output in
 * a file descriptor - if the user specified a valid file to print into
 * NULL - if the user specified an invalid file or an error occured
 */
static file getOutputStream(int argc, char** argv) {
    assert(argv != NULL);
    char* filename = getFlagDescription(argc, argv, OUTPUT_FLAG);
    if(filename == NULL) return stdout; // no file specified
    file output_file = fopen(filename, FILE_WRITE_MODE);
    if(output_file == NULL) return NULL;
    return output_file;
}

/**
 * This function gets the input and output streams for the program
 * it validates the passed command line arguments and updates the IO streams accordingly
 *
 * An error can occur in 2 scenarios:
 * 1. the command line arguments are in an invalid format
 * 2. the requested file cannot be opened
 * In both cases, an error message will be printed using the 'mtmPrintErrorMessage' function
 *
 * @param argc - number of command line arguments
 * @param argv - array of arguments
 * @param input_stream - the destination for the input stream
 * @param output_stream - the destination for the output stream
 * @return
 * PARSER_OK - if no error has occured
 * PARSER_FAILED - if an error did occur
 */
static ParserResult getInputOutputStreams(int argc, char** argv, file* input_stream, file* output_stream) {
    assert(argv != NULL && input_stream != NULL && output_stream != NULL);
    if(!validateCommandLineParameters(argc, argv)) {
        mtmPrintErrorMessage(ERROR_CHANNEL, MTM_INVALID_COMMAND_LINE_PARAMETERS);
        return PARSER_FAILED;
    }

    *input_stream = getInputStream(argc, argv);
    if(*input_stream == NULL) {
        mtmPrintErrorMessage(ERROR_CHANNEL, MTM_CANNOT_OPEN_FILE);
        return PARSER_FAILED;
    }

    *output_stream = getOutputStream(argc, argv);
    if(*output_stream == NULL) {
        if(*input_stream != stdin) {
            fclose(*input_stream);
        }
        mtmPrintErrorMessage(ERROR_CHANNEL, MTM_CANNOT_OPEN_FILE);
        return PARSER_FAILED;
    }

    return PARSER_OK;
}

/**
 * copy a string
 *
 * @param str - a string to copy
 * @return
 * NULL - if an allocation failed
 * a pointer to the newly created string in the case of success
 */
static char* copyString(char* str) {
    assert(str != NULL);
    char* new_string = (char*) malloc(strlen(str) + 1);
    if(new_string == NULL) return NULL;
    strcpy(new_string, str);
    return new_string;
}

/**
 * free a string
 *
 * @param str - the string we want to free
 */
static void freeString(char* str) {
    free(str);
}

/**
 * Checks if a given character is a whitespace
 *
 * A whitespace is one of the following:
 * - space
 * - tab
 * - carriage return
 * - new line
 * - NULL
 *
 * @param ch - the character to check
 * @return
 * true - if the character is a whitespace
 * false - in any other case
 */
static bool isWhitespace(char ch) {
    return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n') || (ch == '\0');
}

/**
 * Return the first word in a string with whitespaces between words
 *
 * The word is dynamically allocated
 *
 * @param str - the string whose first word will be returned
 * @return
 * NULL - if an allocation failed
 * "\0" - if the passed string has no words in it (only whitespaces)
 * a pointer to the first word in any other case
 */
static char* getFirstWord(char* str) {
    assert(str != NULL);

    char* word_start = str;
    // increment the 'word_start' pointer until it points to a non-whitespace character
    // it will eventually point to the first character of the first word
    while(isWhitespace(*word_start) && *word_start != '\0') {
        word_start++;
    }
    char* word_end = word_start;
    // increment the 'word_end' pointer until it points to a whitespace
    // it will eventually point to the first character after the last character of the word
    while(!isWhitespace(*word_end)) {
        word_end++;
    }
    int word_len = word_end - word_start;
    char* word = (char*) malloc(word_len + 1);
    if(word == NULL) return NULL;

    memcpy(word, word_start, word_len);
    word[word_len] = '\0';
    return word;
}

/**
 * Split a string by spaces
 *
 * for example ,if you pass the string " I like    mtm  " to the function,
 * the resulting list will look like this: ["I", "like", "mtm"]
 *
 * @param str - the string to split
 * @return
 * NULL - if an allocation failed
 * a list where each element is a part of the passed string in any other case
 */
static List splitString(char* str) {
    assert(str != NULL);
    List tokens = listCreate((CopyListElement) copyString, (FreeListElement) freeString);
    if(tokens == NULL) return NULL;
    char* current_word = getFirstWord(str);
    if(current_word == NULL) {
        listDestroy(tokens);
        return NULL;
    }
    while(*current_word != '\0') {
        ListResult list_result = listInsertLast(tokens, current_word);
        if(list_result != LIST_SUCCESS) {
            free(current_word);
            listDestroy(tokens);
            return NULL;
        }
        // increment 'str' to point after the word we have added
        while(isWhitespace(*str) && *str != '\0') {
            str++;
        }
        str += strlen(current_word);
        free(current_word);
        current_word = getFirstWord(str);
        if(current_word == NULL) {
            listDestroy(tokens);
            return NULL;
        }
    }
    free(current_word);
    return tokens;
}

/**
 * receives a number in string format and returns its numerical value as an integer
 * for example: "1234" -> 1234
 *
 * 2 assumptions are made:
 * - the string contains a number
 * - the number can be contained within a signed int variable
 *
 * @param str - a string to be converted
 * @return
 * the numerical value of the string in int format
 */
int stringToInt(char* str) {
    assert(str != NULL);
    assert(strlen(str) > 0);

    bool negative = false;
    int num = 0;
    int i = 0;

    if(str[0] == '-') {
        negative = true;
        i = 1;
    }
    for(; str[i] != '\0'; i++) {
        int digit = str[i] - '0';
        num = 10 * num + digit;
    }
    if(negative) return -num;
    else return num;
}

/**
 * prints an error message into the channel defined in the macro 'ERROR_CHANNEL'
 *
 * @param code - the error code whose message will be printed
 */
static void printError(MtmErrorCode code) {
    mtmPrintErrorMessage(ERROR_CHANNEL, code);
}

// ---------------------- Student Command Handlers - Start -----------------------------------------------
/**
 * a handler for the "student add" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are 3 parameters: <id> <first_name> <last_name>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleStudentAddCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 5);    // command + subcommand + 3 parameters

    int id = stringToInt(listGetNext(command_parts));
    char* first_name = listGetNext(command_parts);
    char* last_name = listGetNext(command_parts);

    CourseManagerResult res = addStudent(manager, id, first_name, last_name);
    if(res == COURSE_MANAGER_STUDENT_ALREADY_EXISTS) {
        printError(MTM_STUDENT_ALREADY_EXISTS);
    } else if(res == COURSE_MANAGER_INVALID_PARAMETERS) {
        printError(MTM_INVALID_PARAMETERS);
    } else if(res == COURSE_MANAGER_OUT_OF_MEMORY) {
        printError(MTM_OUT_OF_MEMORY);
        return false;
    }
    return true;
}

/**
 * a handler for the "student remove" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there is 1 parameter: <id>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleStudentRemoveCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 3);    // command + subcommand + 1 parameter

    int id = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = removeStudent(manager, id);
    if(res == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST) {
        printError(MTM_STUDENT_DOES_NOT_EXIST);
    }
    return true;
}

/**
 * a handler for the "student login" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there is 1 parameter: <id>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleStudentLoginCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 3);    // command + subcommand + 1 parameter

    int id = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = logInStudent(manager, id);
    if(res == COURSE_MANAGER_ALREADY_LOGGED_IN) {
        printError(MTM_ALREADY_LOGGED_IN);
    } else if(res == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST) {
        printError(MTM_STUDENT_DOES_NOT_EXIST);
    }
    return true;
}

/**
 * a handler for the "student logout" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are no parameters
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleStudentLogoutCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 2);    // command + subcommand

    CourseManagerResult res = logOutStudent(manager);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    }
    return true;
}

/**
 * a handler for the "student friend_request" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there is 1 parameter: <other_id>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleStudentFriendRequestCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 3);    // command + subcommand + 1 parameter

    int other_id = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = sendFriendRequest(manager, other_id);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    } else if(res == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST) {
        printError(MTM_STUDENT_DOES_NOT_EXIST);
    } else if(res == COURSE_MANAGER_ALREADY_FRIEND) {
        printError(MTM_ALREADY_FRIEND);
    } else if(res == COURSE_MANAGER_ALREADY_REQUESTED) {
        printError(MTM_ALREADY_REQUESTED);
    }
    return true;
}

/**
 * a handler for the "student handle_request" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are 2 parameters: <other_id> <action>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleStudentHandleRequestCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 4);    // command + subcommand + 2 parameters

    int other_id = stringToInt(listGetNext(command_parts));
    char* action = listGetNext(command_parts);

    CourseManagerResult res = handleFriendRequest(manager, other_id, action);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    } else if(res == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST) {
        printError(MTM_STUDENT_DOES_NOT_EXIST);
    } else if(res == COURSE_MANAGER_ALREADY_FRIEND) {
        printError(MTM_ALREADY_FRIEND);
    } else if(res == COURSE_MANAGER_NOT_REQUESTED) {
        printError(MTM_NOT_REQUESTED);
    } else if(res == COURSE_MANAGER_INVALID_PARAMETERS) {
        printError(MTM_INVALID_PARAMETERS);
    }
    return true;
}

/**
 * a handler for the "student unfriend" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there is 1 parameter: <other_id>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleStudentUnfriendCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 3);    // command + subcommand + 1 parameter

    int other_id = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = unFriend(manager, other_id);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    } else if(res == COURSE_MANAGER_STUDENT_DOES_NOT_EXIST) {
        printError(MTM_STUDENT_DOES_NOT_EXIST);
    } else if(res == COURSE_MANAGER_NOT_FRIEND) {
        printError(MTM_NOT_FRIEND);
    }
    return true;
}

/**
 * execute a command that starts with 'student'
 *
 * the list's iterator is assumed to point to the first element
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleStudentCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) >= 2);    // make sure that the list consists of a command and a subcommand

    char* subcommand = listGetNext(command_parts);
    if(!strcmp(subcommand, "add")) {
        return handleStudentAddCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "remove")) {
        return handleStudentRemoveCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "login")) {
        return handleStudentLoginCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "logout")) {
        return handleStudentLogoutCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "friend_request")) {
        return handleStudentFriendRequestCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "handle_request")) {
        return handleStudentHandleRequestCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "unfriend")) {
        return handleStudentUnfriendCommand(manager, command_parts, output_stream);
    } else {    // invalid subcommand, it is assumed that we don't get here
        assert(false);
    }
    return false;   // should never get here
}
// ---------------------- Student Command Handlers - End -----------------------------------------------

// ---------------------- Grade Sheet Command Handlers - Start -----------------------------------------
/**
 * a handler for the "grade_sheet add" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are 4 parameters: <semester> <course_id> <points> <grade>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleGradeSheetAddCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 6);    // command + subcommand + 4 parameters

    int semester = stringToInt(listGetNext(command_parts));
    int course_id = stringToInt(listGetNext(command_parts));
    char* points = listGetNext(command_parts);
    int grade = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = addGrade(manager, semester, course_id, points, grade);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    } else if(res == COURSE_MANAGER_INVALID_PARAMETERS) {
        printError(MTM_INVALID_PARAMETERS);
    }
    return true;
}

/**
 * a handler for the "grade_sheet remove" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are 2 parameters: <semester> <course_id>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleGradeSheetRemoveCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 4);    // command + subcommand + 2 parameters

    int semester = stringToInt(listGetNext(command_parts));
    int course_id = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = removeGrade(manager, semester, course_id);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    } else if(res == COURSE_MANAGER_COURSE_DOES_NOT_EXIST) {
        printError(MTM_COURSE_DOES_NOT_EXIST);
    }
    return true;
}

/**
 * a handler for the "grade_sheet update" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are 2 parameters: <course_id> <new_grade>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleGradeSheetUpdateCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 2);    // command + subcommand + 2 parameters

    int course_id = stringToInt(listGetNext(command_parts));
    int new_grade = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = updateGrade(manager, course_id, new_grade);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    } else if(res == COURSE_MANAGER_COURSE_DOES_NOT_EXIST) {
        printError(MTM_COURSE_DOES_NOT_EXIST);
    } else if(res == COURSE_MANAGER_INVALID_PARAMETERS) {
        printError(MTM_INVALID_PARAMETERS);
    }
    return true;
}

/**
 * execute a command that starts with 'grade_sheet'
 *
 * the list's iterator is assumed to point to the first element
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleGradeSheetCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) >= 2);    // make sure that the list consists of a command and a subcommand

    char* subcommand = listGetNext(command_parts);
    if(!strcmp(subcommand, "add")) {
        return handleGradeSheetAddCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "remove")) {
        return handleGradeSheetRemoveCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "update")) {
        return handleGradeSheetUpdateCommand(manager, command_parts, output_stream);
    } else {    // invalid subcommand, it is assumed that we don't get here
        assert(false);
    }
    return false;   // should never get here
}
// ---------------------- Grade Sheet Command Handlers - End -----------------------------------------

// ---------------------- Report Command Handlers - Start --------------------------------------------
/**
 * a handler for the "report full" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are no parameters
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleReportFullCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 2);    // command + subcommand

    CourseManagerResult res = printFullReport(manager, output_stream);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    }
    return true;
}

/**
 * a handler for the "report clean" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are no parameters
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleReportCleanCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 2);    // command + subcommand

    CourseManagerResult res = printCleanReport(manager, output_stream);
    if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    }
    return true;
}

/**
 * a handler for the "report best" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there is 1 parameter: <amount>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleReportBestCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 3);    // command + subcommand + 1 parameter

    int amount = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = printBestGrades(manager, amount, output_stream);
    if(res == COURSE_MANAGER_INVALID_PARAMETERS) {
        printError(MTM_INVALID_PARAMETERS);
    } else if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    }
    return true;
}

/**
 * a handler for the "report worst" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there is 1 parameter: <amount>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleReportWorstCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 3);    // command + subcommand + 1 parameter

    int amount = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = printWorstGrades(manager, amount, output_stream);
    if(res == COURSE_MANAGER_INVALID_PARAMETERS) {
        printError(MTM_INVALID_PARAMETERS);
    } else if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    }
    return true;
}

/**
 * a handler for the "report reference" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are 2 parameters: <course_id> <amount>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleReportReferenceCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 4);    // command + subcommand + 2 parameters

    int course_id = stringToInt(listGetNext(command_parts));
    int amount = stringToInt(listGetNext(command_parts));

    CourseManagerResult res = printReferenceSources(manager, course_id, amount, output_stream);
    if(res == COURSE_MANAGER_INVALID_PARAMETERS) {
        printError(MTM_INVALID_PARAMETERS);
    } else if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    }
    return true;
}

/**
 * a handler for the "report faculty_request" command
 * it is assumed that the list's iterator points to the subcommand part (2nd word)
 * we also assume that there are 2 parameters: <course_id> <request>
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleReportFacultyRequestCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) == 4);    // command + subcommand + 2 parameters

    int course_id = stringToInt(listGetNext(command_parts));
    char* request = listGetNext(command_parts);

    CourseManagerResult res = sendFacultyReqeust(manager, course_id, request, output_stream);
    if(res == COURSE_MANAGER_INVALID_PARAMETERS) {
        printError(MTM_INVALID_PARAMETERS);
    } else if(res == COURSE_MANAGER_NOT_LOGGED_IN) {
        printError(MTM_NOT_LOGGED_IN);
    } else if(res == COURSE_MANAGER_COURSE_DOES_NOT_EXIST) {
        printError(MTM_COURSE_DOES_NOT_EXIST);
    }
    return true;
}

/**
 * execute a command that starts with 'report'
 *
 * the list's iterator is assumed to point to the first element
 *
 * @param manager - the course manager
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output goes
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleReportCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    assert(listGetSize(command_parts) >= 2);    // make sure that the list consists of a command and a subcommand

    char* subcommand = listGetNext(command_parts);
    if(!strcmp(subcommand, "full")) {
        return handleReportFullCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "clean")) {
        return handleReportCleanCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "best")) {
        return handleReportBestCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "worst")) {
        return handleReportWorstCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "reference")) {
        return handleReportReferenceCommand(manager, command_parts, output_stream);
    } else if(!strcmp(subcommand, "faculty_request")) {
        return handleReportFacultyRequestCommand(manager, command_parts, output_stream);
    } else {    // invalid subcommand, it is assumed that we don't get here
        assert(false);
    }
    return false;   // should never get here
}
// ---------------------- Report Command Handlers - End --------------------------------------------

/**
 * execute a command
 *
 * @param manager - the course manager that we are dealing with
 * @param command_parts - the command as a list of strings
 * @param output_stream - the stream where the output will go to
 *
 * @return
 * true - if no memory errors occured
 * false - if a memory error occured
 */
static bool handleCommand(CourseManager manager, List command_parts, file output_stream) {
    assert(manager != NULL && command_parts != NULL && output_stream != NULL);
    if(listGetSize(command_parts) == 0) return true;     // if the line consists only of whitespaces, then do nothing
    char* first_word = listGetFirst(command_parts);
    if(first_word[0] == COMMENT_PREFIX) return true;     // if the line is a comment then do nothing
    // if it got here then the line is a potential command
    if(!strcmp(first_word, "student")) {
        return handleStudentCommand(manager, command_parts, output_stream);
    } else if(!strcmp(first_word, "grade_sheet")) {
        return handleGradeSheetCommand(manager, command_parts, output_stream);
    } else if(!strcmp(first_word, "report")) {
        return handleReportCommand(manager, command_parts, output_stream);
    } else {    // illegal command, we can assume that it doesn't get here
        assert(false);
    }
    return false;   // should never get here
}

/**
 * Iterates over the input stream and executes command
 *
 * @param input_stream - the stream to get the commands from
 * @param output_stream - the stream where the output of the program will go
 */
static void executeCommands(file input_stream, file output_stream) {
    assert(input_stream != NULL && output_stream != NULL);

    CourseManager manager = courseManagerCreate();
    if(manager == NULL) {
        printError(MTM_OUT_OF_MEMORY);
        return;
    }

    char buffer[MAX_LEN + 1] = "";
    while(fgets(buffer, MAX_LEN, input_stream) != NULL) {   // iterate over the lines of the input stream
        List command_parts = splitString(buffer);
        if(command_parts == NULL) {
            printError(MTM_OUT_OF_MEMORY);
            listDestroy(command_parts);
            destroyCourseManager(manager);
            return;
        } else {
            bool success = handleCommand(manager, command_parts, output_stream);
            listDestroy(command_parts);
            if(!success) {
                destroyCourseManager(manager);
                return;
            }
        }
    }
    destroyCourseManager(manager);
}

int main(int argc, char** argv) {
    file input_stream = NULL, output_stream = NULL;
    ParserResult result = getInputOutputStreams(argc, argv, &input_stream, &output_stream);
    if(result == PARSER_FAILED) return 0;

    executeCommands(input_stream, output_stream);

    if(input_stream != stdin) {
        fclose(input_stream);
    }
    if(output_stream != stdout) {
        fclose(output_stream);
    }

    return 0;
}