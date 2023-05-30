#include <stdbool.h>
#include <stdio.h>

void print_usage(char *progname) {
  printf("Usage: %s [-bEnstuv] [file_name]\n", progname);
}

void print_usage_grep(char *progname) {
  printf("Usage: %s [-eivclnhfos] pattern [file_name]\n", progname);
}

void file_check(int file_count) {
  if (file_count == 0) {
    printf("enter file name");
  }
}

int file_read_check_grep(char *arr, bool flag_s) {
  int flag = 1;
  FILE *fp = fopen(arr, "r");
  if (fp == NULL && flag_s != 1) {
    printf("%s: No such file or directory\n", arr);
    flag = 0;
  } else if (fp == NULL && flag_s == 1) {
    flag = 0;
  } else
    fclose(fp);
  return flag;
}

int file_read_check(char *arr) {
  int flag = 1;
  FILE *fp = fopen(arr, "r");

  if (fp == NULL) {
    printf("%s: No such file or directory\n", arr);
    flag = 0;
  } else
    fclose(fp);
  return flag;
}
