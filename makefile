CC = gcc
OBJS = course_manager.o student.o semester.o grade.o main.o
TEST_OBJS = course_manager_test.o student_test.o semester_test.o grade_test.o list_mtm_test.o list_mtm.o
EXEC = mtm_cm
CFLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -L. -lmtm

tests : $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) course_manager.o student.o semester.o grade.o course_manager_test.o -o course_manager_test -L. -lmtm
	$(CC) $(CFLAGS) student.o semester.o grade.o grade_test.o -o grade_test -L. -lmtm
	$(CC) $(CFLAGS) student.o semester.o grade.o semester_test.o -o semester_test -L. -lmtm
	$(CC) $(CFLAGS) student.o semester.o grade.o student_test.o -o student_test -L. -lmtm
	$(CC) $(CFLAGS) list_mtm.o list_mtm_test.o -o list_mtm_test
	
course_manager.o: course_manager.c course_manager.h list.h set.h \
 mtm_ex3.h student.h libmtm.a
grade.o: grade.c grade.h list.h set.h mtm_ex3.h libmtm.a
main.o: main.c list.h mtm_ex3.h course_manager.h set.h student.h libmtm.a
semester.o: semester.c semester.h set.h list.h mtm_ex3.h grade.h \
 student.h libmtm.a
student.o: student.c student.h set.h mtm_ex3.h semester.h list.h grade.h libmtm.a
#tests objects
course_manager_test.o: course_manager_test.c test_utilities.h \
 course_manager.h list.h set.h mtm_ex3.h student.h libmtm.a
	$(CC) $(CFLAGS) -c ./tests/course_manager_test.c -o course_manager_test.o -L. -lmtm
grade_test.o: grade_test.c test_utilities.h grade.h list.h set.h \
 mtm_ex3.h libmtm.a
	$(CC) $(CFLAGS) -c ./tests/grade_test.c -o grade_test.o -L. -lmtm
semester_test.o: semester_test.c test_utilities.h semester.h set.h \
 list.h mtm_ex3.h grade.h libmtm.a
	$(CC) $(CFLAGS) -c ./tests/semester_test.c -o semester_test.o -L. -lmtm
student_test.o: student_test.c test_utilities.h student.h set.h \
 mtm_ex3.h libmtm.a
	$(CC) $(CFLAGS) -c ./tests/student_test.c -o student_test.o -L. -lmtm
list_mtm.o: ./list_mtm/list_mtm.c ./list_mtm/list_mtm.h
	$(CC) $(CFLAGS) -c ./list_mtm/list_mtm.c -o list_mtm.o
list_mtm_test.o: ./tests/list_mtm_test.c  ./list_mtm/list_mtm.c ./list_mtm/list_mtm.h ./tests/test_utilities.h
	$(CC) $(CFLAGS) -c ./tests/list_mtm_test.c -o list_mtm_test.o

	
clean:
	rm -f $(OBJS) $(TEST_OBJS)