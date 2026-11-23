#ifndef UTILS_H
#define UTILS_H

#define line_size 256
#define hours_in_day 24

void remove_newline(char *text);
int read_line(char *buffer, int size);
int read_int(const char *prompt, int min, int max, int *result);
int read_text(const char *prompt, char *buffer, int max_len);
int calc_day(int day, int hour, int hours);
int calc_hour(int hour, int hours);
void print_time(int day, int hour);
int unpack_line(const char *packed, char *result, int result_size);
int unpack_map_file(const char *in_name, const char *out_name);
void print_file(const char *filename);
void menu_unpack_map(void);

#endif
