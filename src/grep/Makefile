CC=gcc -std=c11 -D_GNU_SOURCE
CFLAGS= -Wall -Werror -Wextra 
GREP = s21_grep.c

all: s21_grep
s21_grep: common.o s21_grep
	$(CC) $(CFLAGS) $(GREP) common.o -o s21_grep

s21_grep.c:
	$(CC) $(CFLAGS) -o s21_grep.c 

common.o: common.c
	$(CC) $(CFLAGS) -c ../common/common.c

common.c: 
	$(CC) $(CFLAGS) -c ../common/common.c

clean:
	rm -f *.o s21_grep
rebuild:
	make clean
	make all
