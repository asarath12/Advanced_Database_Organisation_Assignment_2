CC = gcc
CFLAGS  = -g -Wall
LDFLAGS = -pthread
 


run: run_test1

test1: test_assign3_1.o storage_mgr.o dberror.o buffer_mgr.o buffer_mgr_stat.o expr.o record_mgr.o rm_serializer.o
	$(CC) $(CFLAGS) -o test1 test_assign3_1.o storage_mgr.o dberror.o buffer_mgr.o buffer_mgr_stat.o expr.o record_mgr.o rm_serializer.o -lm $(LDFLAGS)

test_assign3_1.o: test_assign3_1.c dberror.h storage_mgr.h test_helper.h buffer_mgr.h buffer_mgr_stat.h expr.o record_mgr.o rm_serializer.o
	$(CC) $(CFLAGS) -c test_assign3_1.c  -lm

buffer_mgr_stat.o: buffer_mgr_stat.c buffer_mgr_stat.h buffer_mgr.h
	$(CC) $(CFLAGS) -c buffer_mgr_stat.c

buffer_mgr.o: buffer_mgr.c buffer_mgr.h dt.h storage_mgr.h
	$(CC) $(CFLAGS) -c buffer_mgr.c -pthread

storage_mgr.o: storage_mgr.c storage_mgr.h 
	$(CC) $(CFLAGS) -c storage_mgr.c -lm

dberror.o: dberror.c dberror.h 
	$(CC) $(CFLAGS) -c dberror.c

expr.o: expr.c expr.h
	$(CC) $(CFLAGS) -c expr.c

record_mgr.o: record_mgr.c record_mgr.h
	$(CC) $(CFLAGS) -c record_mgr.c

rm_serializer.o: rm_serializer.c
	$(CC) $(CFLAGS) -c rm_serializer.c

clean: 
	$(RM) test1  *.o *~

run_test1:
	./test1
