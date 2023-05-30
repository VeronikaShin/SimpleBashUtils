#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void flags(int argc, char *argv[], char *pattern, char *file_pattern,
           bool *ignore_case, bool *pattern_e, bool *invert_match,
           bool *count_lines, bool *file_with_matching, bool *line_number,
           bool *flag_h, bool *flag_s, bool *flag_o, bool *flag_f,
           bool *flag_err);
void options(int argc, char *argv[], char *pattern, char *file_name,
             bool flag_f, bool flag_s, bool ignore_case, bool pattern_e,
             bool invert_match, bool count_lines, bool file_with_matching,
             bool line_number, bool flag_h, bool flag_o, bool flag_err,
             int f_count, int count);
void open(char *file_name, char *pattern, bool ignore_case, bool pattern_e,
          bool invert_match, bool count_lines, bool file_with_matching,
          bool line_number, bool flag_h, bool flag_s, bool flag_o, bool flag_f,
          bool flag_err, int f_count, int len);
void print_line(char *file_name, char *line, int f_count, FILE *fp,
                bool flag_h);
int pattern_check(char *pattern, char *line, bool ignore_case);
int file_count(int argc, char *argv[], bool flag_f);
void eof_check(FILE *fp, char *line);
void print_str(char *line, char *pattern, char *file_name, int f_count,
               bool ignore_case, int cnt_lines, bool line_number, bool flag_h);
int check_flagf(char *file_pattern, char *pattern);
int printf_pattern(int count, char *line, char *pattern, bool ignore_case,
                   char *pattern_f);

int main(int argc, char *argv[]) {
  bool ignore_case = false, pattern_e = false, invert_match = false,
       count_lines = false, file_with_matching = false, line_number = false,
       flag_h = false, flag_s = false, flag_o = false, flag_f = false,
       flag_err = false;
  char pattern[BUFSIZ] = {0}, file_pattern[BUFSIZ] = {0},
       file_name[BUFSIZ] = {0};
  int count;

  flags(argc, argv, pattern, file_pattern, &ignore_case, &pattern_e,
        &invert_match, &count_lines, &file_with_matching, &line_number, &flag_h,
        &flag_s, &flag_o, &flag_f, &flag_err);

  int f_count = file_count(argc, argv, flag_f);
  if (flag_f) {
    count = check_flagf(file_pattern, pattern);
  }
  options(argc, argv, pattern, file_name, flag_f, flag_s, ignore_case,
          pattern_e, invert_match, count_lines, file_with_matching, line_number,
          flag_h, flag_o, flag_err, f_count, count);

  return 0;
}

void flags(int argc, char *argv[], char *pattern, char *file_pattern,
           bool *ignore_case, bool *pattern_e, bool *invert_match,
           bool *count_lines, bool *file_with_matching, bool *line_number,
           bool *flag_h, bool *flag_s, bool *flag_o, bool *flag_f,
           bool *flag_err) {
  if (argv[1][0] == '-') {
    int opt = 0;
    opterr = 0;  // отключаем вывод ошибок в stderr
    const char *s_options = "e:ivclnhsf:o";
    while ((opt = getopt(argc, argv, s_options)) != -1) {
      switch (opt) {
        case 'e':
          *pattern_e = true;
          if (strlen(pattern) == 0) {
            strcat(pattern, optarg);
          } else {
            strcat(pattern, "|");
            strcat(pattern, optarg);
          }
          break;
        case 'i':
          *ignore_case = true;
          break;
        case 'v':
          *invert_match = true;
          break;
        case 'c':
          *count_lines = true;
          break;
        case 'l':
          *file_with_matching = true;
          break;
        case 'n':
          *line_number = true;
          break;
        case 'h':
          *flag_h = true;
          break;
        case 's':
          *flag_s = true;
          break;
        case 'o':
          *flag_o = true;
          break;
        case 'f':
          *flag_f = true;
          snprintf(file_pattern, BUFSIZ, "%s", optarg);
          break;
        default:
          *flag_err = true;
          break;
      }
    }
  }
}

void options(int argc, char *argv[], char *pattern, char *file_name,
             bool flag_f, bool flag_s, bool ignore_case, bool pattern_e,
             bool invert_match, bool count_lines, bool file_with_matching,
             bool line_number, bool flag_h, bool flag_o, bool flag_err,
             int f_count, int count) {
  int counter = 0;
  for (int i = 1; i < argc; i++) {
    if ((strrchr(argv[i], '.') == 0) && strrchr(&argv[i][0], '-') == 0 &&
        pattern_e == 0) {
      snprintf(pattern, BUFSIZ, "%s", argv[i]);
    }
    if (strrchr(argv[i], '.') != 0 && flag_f == 1 && counter == 0) {
      i++;
      counter++;
    }
    if (strrchr(argv[i], '.') != 0) {
      snprintf(file_name, BUFSIZ, "%s", argv[i]);
      if (file_read_check_grep(file_name, flag_s)) {
        open(file_name, pattern, ignore_case, pattern_e, invert_match,
             count_lines, file_with_matching, line_number, flag_h, flag_s,
             flag_o, flag_f, flag_err, f_count, count);
      }
    }
  }
}

int file_count(int argc, char *argv[], bool flag_f) {
  int file_count = 0;
  for (int i = 1; i < argc; i++) {
    if (strrchr(argv[i], '.') != 0) file_count++;
  }
  if (flag_f) file_count--;
  return file_count;
}

void open(char *file_name, char *pattern, bool ignore_case, bool pattern_e,
          bool invert_match, bool count_lines, bool file_with_matching,
          bool line_number, bool flag_h, bool flag_s, bool flag_o, bool flag_f,
          bool flag_err, int f_count, int count) {
  int counter = 0, flag_l = 0, cnt_lines = 0, newline_cnt = 0;
  char line[BUFSIZ], pattern_f[BUFSIZ] = {0};
  FILE *fp = fopen(file_name, "r");
  fseek(fp, 0, SEEK_SET);
  while (fgets(line, sizeof(line), fp) != NULL) {
    cnt_lines++;
    newline_cnt++;
    if (flag_f) {
      int match = 0;
      if (count == -2) {
        print_line(file_name, line, f_count, fp, flag_h);
      } else {
        match = printf_pattern(count, line, pattern, ignore_case, pattern_f);
        if (match) print_line(file_name, line, f_count, fp, flag_h);
      }
    } else {
      if (pattern_check(pattern, line, ignore_case) == 0) {
        if (ignore_case == 0 && pattern_e == 0 && invert_match == 0 &&
            count_lines == 0 && file_with_matching == 0 && line_number == 0 &&
            flag_h == 0 && flag_s == 0 && flag_o == 0 && flag_f == 0 &&
            flag_err == 0) {
          print_line(file_name, line, f_count, fp, flag_h);
        }
        if ((ignore_case || pattern_e || flag_h || flag_s) &&
            count_lines == 0 && invert_match == 0 && file_with_matching == 0 &&
            line_number == 0 && flag_o == 0) {
          print_line(file_name, line, f_count, fp, flag_h);
        }
        if (flag_o && invert_match == 0 && count_lines == 0 &&
            file_with_matching == 0) {
          print_str(line, pattern, file_name, f_count, ignore_case, cnt_lines,
                    line_number, flag_h);
        }
        if (count_lines && invert_match == 0) {
          counter++;
        }
        if (file_with_matching) {
          flag_l = 1;
        }
        if (line_number && f_count == 1 && invert_match == 0 &&
            count_lines == 0 && file_with_matching == 0 && flag_o == 0) {
          printf("%d:%s", cnt_lines, line);
          eof_check(fp, line);
        }
        if (line_number && f_count > 1 && invert_match == 0 &&
            count_lines == 0 && file_with_matching == 0 && flag_h == 0 &&
            flag_o == 0) {
          printf("%s:%d:%s", file_name, cnt_lines, line);
          eof_check(fp, line);
        }
        if (line_number && f_count > 1 && invert_match == 0 &&
            count_lines == 0 && file_with_matching == 0 && flag_h == 1 &&
            flag_o == 0) {
          printf("%d:%s", cnt_lines, line);
          eof_check(fp, line);
        }
      } else if (invert_match) {
        counter++;
        if (file_with_matching == 1) flag_l = 1;
        if (count_lines == 0 && file_with_matching == 0 && line_number == 0)
          print_line(file_name, line, f_count, fp, flag_h);
        if (line_number && f_count == 1) {
          printf("%d:%s", cnt_lines, line);
          eof_check(fp, line);
        }
        if (line_number && f_count > 1) {
          printf("%s:%d:%s", file_name, cnt_lines, line);
          eof_check(fp, line);
        }
      }
    }
  }
  if (count_lines && (f_count == 1) && flag_h == 0) {
    if (flag_l == 1 && counter > 0) counter = 1;
    printf("%d\n", counter);
  }
  if (count_lines && (f_count > 1) && flag_h == 0) {
    if (flag_l == 1 && counter > 0) counter = 1;
    printf("%s:%d\n", file_name, counter);
  }
  if (count_lines && (f_count == 1) && flag_h == 1) {
    printf("%d\n", counter);
  }
  if (count_lines && (f_count > 1) && flag_h == 1) {
    printf("%d\n", counter);
  }
  if (file_with_matching && flag_l == 1) {
    printf("%s\n", file_name);
  }
  fclose(fp);
}

void print_line(char *file_name, char *line, int f_count, FILE *fp,
                bool flag_h) {
  if (f_count == 1) {
    printf("%s", line);
    eof_check(fp, line);
  } else if (f_count > 1 && flag_h == 0) {
    printf("%s:%s", file_name, line);
    eof_check(fp, line);
  } else if (flag_h && f_count > 1) {
    printf("%s", line);
    eof_check(fp, line);
  }
}

void eof_check(FILE *fp, char *line) {
  if (feof(fp)) {
    if (strlen(line) - 1 != '\n') {
      printf("\n");
    }
  }
}

int pattern_check(char *pattern, char *line, bool ignore_case) {
  int flags, res;
  regex_t regex;
  if (ignore_case) {
    flags = REG_ICASE;
  } else
    flags = REG_EXTENDED;
  res = regcomp(&regex, pattern, flags);
  regmatch_t pmatch[BUFSIZ];
  res = regexec(&regex, line, 1, pmatch, flags);
  regfree(&regex);
  return res;
}

void print_str(char *line, char *pattern, char *file_name, int f_count,
               bool ignore_case, int cnt_lines, bool line_number, bool flag_h) {
  regex_t regex;
  int flags = 0;
  int counter = 0;
  if (ignore_case) flags = REG_ICASE;
  regcomp(&regex, pattern, flags);
  regmatch_t pmatch[BUFSIZ];
  char match[BUFSIZ];
  char *p = line;
  int p_count = 0;
  while (regexec(&regex, p, 1, pmatch, 0) == 0) {
    int len = pmatch[0].rm_eo - pmatch[0].rm_so;
    memcpy(match, p + pmatch[0].rm_so, len);
    // копирует до опред момента, а не до символа конца строки как strcpy
    match[len] = '\0';
    if (f_count > 1 && p_count == 0 && line_number == 0 && flag_h == 0) {
      printf("%s:%s\n", file_name, match);
      p_count++;
    } else if (f_count > 1 && p_count == 0 && line_number == 1 && flag_h == 0) {
      printf("%s:%d:%s\n", file_name, cnt_lines, match);
      p_count++;
    } else if (f_count == 1 && p_count == 0 && line_number == 1 &&
               counter == 0 && flag_h == 0) {
      printf("%d:%s\n", cnt_lines, match);
      counter++;
    } else
      printf("%s\n", match);
    p += pmatch[0].rm_eo;
    // переход на след символы, после нашей подстроки
  }
  regfree(&regex);
}

int check_flagf(char *file_pattern, char *pattern) {
  FILE *fpf = fopen(file_pattern, "r");
  int ch, i = 0, count = 0, prev = '|';
  if (fpf != NULL) {
    while ((ch = fgetc(fpf)) != EOF) {
      if (ch == '\n' && prev != '\n') {
        pattern[i] = '|';
        i++;
        count++;
        prev = ch;
      } else if (ch == '\n' && prev == '\n') {
        count = -2;
        break;
      } else {
        pattern[i] = ch;
        i++;
        prev = ch;
      }
    }
    pattern[i] = '|';
  } else
    count = -1;
  fclose(fpf);
  return count;
}

int printf_pattern(int count, char *line, char *pattern, bool ignore_case,
                   char *pattern_f) {
  char *pattern_copy = strdup(pattern);
  int match = 0;
  for (int j = 0; j <= count; j++) {
    char *ptr = strchr(pattern_copy, '|');
    if (ptr != NULL) {
      strncpy(pattern_f, pattern_copy, ptr - pattern_copy);
      pattern_f[ptr - pattern_copy] = '\0';
      if (pattern_check(pattern_f, line, ignore_case) == 0) {
        match = 1;
      }
      // для удаления уже записанного слова из pattern
      char *read_ptr = pattern_copy;
      char *write_ptr = pattern_copy;
      int s_count = 0;
      while (*read_ptr != '\0') {
        if (*read_ptr == '|' && s_count == 0) {
          read_ptr++;
          s_count++;
          continue;
        }
        if (strchr(pattern_f, *read_ptr) == NULL) {
          *write_ptr = *read_ptr;
          write_ptr++;
        }
        read_ptr++;
      }
      *write_ptr = '\0';
    }
    memset(pattern_f, 0, BUFSIZ * sizeof(char));
  }
  free(pattern_copy);
  return match;
}
