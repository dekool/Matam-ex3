cmake_minimum_required(VERSION 3.8)
project(ex3)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c student.h course_manager.h course.h semester_grade.h student.c course_manager.c course.c semester_grade.c semester.h semester.c grade.h grade.c)
add_executable(ex3 ${SOURCE_FILES})