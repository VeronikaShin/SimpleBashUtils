#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void open_file(int argc, char **argv, bool display_ends, bool display_ends_gnu,
               bool squeeze_empty, bool display_tabs, bool display_tabs_gnu,
               bool display_nonempty, bool display_number);
void open_n(bool display_number, FILE *file_pointer);
void open_b(bool display_nonempty, FILE *file_pointer);
void open_tab(bool x, FILE *file_pointer);
void open_e(bool x, FILE *file_pointer);
void open_t_e_v(bool display_ends, bool display_tabs, FILE *file_pointer);
void open_s(bool squeeze_empty, FILE *file_pointer);

int main(int argc, char *argv[]) {
  bool display_ends = false, squeeze_empty = false, display_tabs = false,
       display_tabs_gnu = false, display_nonempty = false,
       display_number = false, display_ends_gnu = false;
  int file_count = 0;

  for (int x = 1; x < argc; x++) {
    if ((argv[x][0] == '-') && argv[x][1] != '-') {
      for (int j = 1; argv[x][j] != '\0'; j++) {
        switch (argv[x][j]) {
          case 'b':
            display_nonempty = true;
            break;
          case 'e':
            display_ends = true;
            break;
          case 'E':
            display_ends_gnu = true;
            break;
          case 's':
            squeeze_empty = true;
            break;
          case 't':
            display_tabs = true;
            break;
          case 'T':
            display_tabs_gnu = true;
            break;
          case 'n':
            display_number = true;
            break;
          default:
            printf("Invalid option: -%c\n", argv[x][j]);
            print_usage(argv[0]);
        }
      }
    }
    if (argv[x][0] == '-' && argv[x][1] == '-') {
      for (int k = 1; k < argc; k++) {
        if (strcmp(argv[k], "--number-nonblank") == 0)
          display_nonempty = true;
        else if (strcmp(argv[k], "--number") == 0)
          display_number = true;
        else if (strcmp(argv[k], "--squeeze-blank") == 0)
          squeeze_empty = true;
        else if (argv[k][0] != '-')
          continue;
        else {
          printf("Invalid option: %s\n", argv[k]);
          print_usage(argv[0]);
        }
      }
    } else {
      file_count++;
    }  // значит нам передали не флаг, а имя файла
  }

  file_check(file_count);
  open_file(argc, argv, display_ends, display_ends_gnu, squeeze_empty,
            display_tabs, display_tabs_gnu, display_nonempty, display_number);
}

void open_file(int argc, char **argv, bool display_ends, bool display_ends_gnu,
               bool squeeze_empty, bool display_tabs, bool display_tabs_gnu,
               bool display_nonempty, bool display_number) {
  for (int i = 1; i < argc; i++) {
    int x = 0, c;
    if (argv[i][0] != '-') {
      x = file_read_check(argv[i]);  // проверка на корректность ввода(проверку
      // на пустоту решила не делать, как и cat)
    };
    if ((x)) {
      FILE *fp = fopen(argv[i], "r+");
      while ((c = fgetc(fp)) != EOF) {
        if (squeeze_empty) {
          open_s(squeeze_empty, fp);

        } else if (display_tabs) {
          open_t_e_v(display_ends, display_tabs, fp);

        } else if (display_tabs_gnu) {
          open_tab(display_tabs_gnu, fp);

        } else if (display_nonempty) {
          open_b(display_nonempty, fp);

        } else if (display_ends) {
          open_t_e_v(display_ends, display_tabs, fp);

        } else if (display_ends_gnu) {
          open_e(display_ends_gnu, fp);

        } else if (display_number)
          open_n(display_number, fp);

        else {
          putchar(c);
        }
      }
      fclose(fp);
    }
  }
}

void open_n(bool display_number, FILE *file_pointer) {
  if (display_number) {
    char line[BUFSIZ];
    int line_number = 1;
    rewind(file_pointer);
    while (fgets(line, sizeof(line), file_pointer)) {
      printf("%6d	%s", line_number, line);
      line_number++;
    }
  }
}

void open_b(bool display_nonempty, FILE *file_pointer) {
  if (display_nonempty) {
    char line[BUFSIZ];
    int line_num = 1;
    rewind(file_pointer);
    while (fgets(line, sizeof(line), file_pointer)) {
      if (line[0] != '\n') {
        printf("%6d	%s", line_num, line);
        line_num++;
      } else
        putchar('\n');
    }
  }
}

void open_s(bool squeeze_empty, FILE *file_pointer) {
  int prev_char = 1, c;
  rewind(file_pointer);
  if (squeeze_empty) {
    while ((c = fgetc(file_pointer)) != EOF) {
      if (c == '\n' && prev_char >= 2) {
        continue;
      } else if ((c == '\n') && (prev_char < 2)) {
        putchar(c);
        prev_char++;
      } else {
        prev_char = 0;
        putchar(c);
      }
    }
  }
}

void open_tab(bool x, FILE *file_pointer) {
  int c;
  rewind(file_pointer);
  if (x) {
    while ((c = fgetc(file_pointer)) != EOF) {
      if (c == '\t') {
        putchar('^');
        putchar('I');
      } else
        putchar(c);
    }
  }
}

void open_e(bool x, FILE *file_pointer) {
  int c;
  rewind(file_pointer);
  if (x) {
    while ((c = fgetc(file_pointer)) != EOF) {
      if (c == '\n') {
        putchar('$');
        putchar('\n');
      } else
        putchar(c);
    }
  }
}

// void open_t_e_v(bool display_ends, bool display_tabs, FILE *file_pointer) {
//   int c;
//   rewind(file_pointer);
//   while ((c = getc(file_pointer)) != EOF) {
//     if ((c == '\n') && (display_ends)) {
//       putchar('$');
//     }
//     if ((c == '\t') && (display_tabs)) {
//       printf("^I");

//     } else if (c == 127)
//       printf("^?");
//     else if ((c < 32) && (c != 9) && (c != 10)) {
//       printf("^%c", c + 64);
//     } else if ((c > 127) && (c <= 160)) {
//       if (c > 127)
//         printf("M-^%c", c - 64);
//       else
//         printf("^%c", c + 64);
//     } else
//       putchar(c);
//   }
// }
void open_t_e_v(bool display_ends, bool display_tabs, FILE *file_pointer) {
  int c;
  rewind(file_pointer);
  while ((c = getc(file_pointer)) != EOF) {
    if ((c == '\n') && (display_ends)) {
      putchar('$');
      putchar(c);
    } else if ((c == '\t') && (display_tabs))
      printf("^I");
    else if ((c > 127) && (c < 160))
      printf("M-^%c", c - 64);
    else if (c == 127)
      printf("^?");
    else if ((c != 9) && (c != 10) && (c < 32))
      printf("^%c", c + 64);
    else
      putchar(c);
  }
}
