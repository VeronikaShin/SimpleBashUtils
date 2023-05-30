CC=gcc -std=c11 -D_GNU_SOURCE
CFLAGS= -Wall -Werror -Wextra 

all: s21_cat
s21_cat: common.o s21_cat.o
	$(CC) $(CFLAGS) s21_cat.c common.o -o s21_cat

s21_cat.c:
	$(CC) $(CFLAGS) -o s21_cat.c 

common.o: common.c
	$(CC) $(CFLAGS) -c ../common/common.c

common.c: 
	$(CC) $(CFLAGS) -c ../common/common.c

# test:
# 	chmod 777 test.sh
# 	test.sh

clean:
	rm -f *.o s21_cat
rebuild:
	make clean
	make all
