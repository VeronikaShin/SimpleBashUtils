#ifndef CHECK
#define CHECK

void file_check(int file_count);
int file_read_check(char *arr);
int file_read_check_grep(char *arr, bool flag_s);
void print_usage(char *progname);
void print_usage_grep(char *progname);

#endif