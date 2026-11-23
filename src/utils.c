#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define input_file "input.txt"
#define output_file "output.txt"
#define map_line_size 512
#define no_width (-1)
#define unpack_error (-1)
#define map_err_no_input (-1)
#define map_err_no_output (-2)

void remove_newline(char *text)
{
    int len = strlen(text);

    while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r')) {
        text[len - 1] = '\0';
        len--;
    }
}

int is_digit(char c) /* ЗАДАНИЕ ПО ВАРИАНТУ(проверка на цифру) */
{
    return c >= '0' && c <= '9';
}

int read_line(char *buffer, int size)
{
    int ch;
    for (;;) {
        if (fgets(buffer, size, stdin) == NULL) {
            return 0;
        }
        if (strchr(buffer, '\n') == NULL && !feof(stdin)) {
            ch = getchar();
            if (ch != '\n' && ch != EOF) {
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }
                printf("Ошибка: слишком длинная строка. Повторите ввод: ");
                continue;
            }
        }
        remove_newline(buffer);
        return 1;
    }
}

int read_int(const char *prompt, int min, int max)
{
    char line[line_size];
    long value;
    char *end;
    char *start;

    while (1) {
        printf("%s (%d-%d): ", prompt, min, max);
        if (!read_line(line, line_size)) {
            printf("\nВвод закрыт. Выход из программы.\n");
            exit(0);
        }
        errno = 0;
        start = line;
        while (isspace((unsigned char)*start)) {
            start++;
        }
        value = strtol(start, &end, 10);
        while (isspace((unsigned char)*end)) {
            end++;
        }
        if (end == start || *end != '\0' || errno == ERANGE) {
            printf("Ошибка: нужно ввести число!\n");
        } else if (value < min || value > max) {
            printf("Ошибка: число должно быть от %d до %d!\n", min, max);
        } else {
            return (int)value;
        }
    }
}

void read_text(const char *prompt, char *buffer, int max_len)
{
    char line[line_size];

    while (1) {
        printf("%s", prompt);
        if (!read_line(line, line_size)) {
            printf("\nВвод закрыт. Выход из программы.\n");
            exit(0);
        }
        if (strlen(line) == 0) {
            printf("Ошибка: строка не должна быть пустой!\n");
        } else if ((int)strlen(line) >= max_len) {
            printf("Ошибка: не длиннее %d символов (байт)!\n", max_len - 1);
        } else {
            strcpy(buffer, line);
            return;
        }
    }
}

int calc_day(int day, int hour, int hours)
{
    return day + (hour + hours) / hours_in_day;
}

int calc_hour(int hour, int hours)
{
    return (hour + hours) % hours_in_day;
}

void print_time(int day, int hour)
{
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

int unpack_line(const char *packed, char *result, int result_size)
{
    int i = 0;       
    int len = 0;     
    int count;       
    int k;
    char symbol;

    while (packed[i] != '\0') {
        if (!is_digit(packed[i])) {
            return unpack_error;
        }
        count = 0;
        while (is_digit(packed[i])) {
            count = count * 10 + (packed[i] - '0');
            if (count >= result_size) {
                return unpack_error;
            }
            i++;
        }

        if (packed[i] == '\0') {
            return unpack_error;
        }
        symbol = packed[i];
        i++;

        if (count == 0 || len + count >= result_size) {
            return unpack_error;
        }
        for (k = 0; k < count; k++) {
            result[len] = symbol;
            len++;
        }
    }

    result[len] = '\0';
    return len;
}

int unpack_map_file(const char *in_name, const char *out_name)
{
    FILE *in;
    FILE *out;
    char packed[map_line_size];
    char unpacked[map_line_size];
    int line_number = 0;
    int width = no_width;    /* длина первой правильной строки */
    int rows = 0;
    int errors = 0;
    int len;

    in = fopen(in_name, "r");
    if (in == NULL) {
        return map_err_no_input;
    }
    out = fopen(out_name, "w");
    if (out == NULL) {
        fclose(in);
        return map_err_no_output;
    }

    while (fgets(packed, map_line_size, in) != NULL) {
        line_number++;
        remove_newline(packed);
        if (strlen(packed) == 0) {
            continue;    /* пустые строки пропускаем */
        }

        len = unpack_line(packed, unpacked, map_line_size);
        if (len == unpack_error) {
            fprintf(out, "Ошибка в строке %d: неверная запись \"%s\"\n", line_number, packed);
            errors++;
        } else if (width != no_width && len != width) {
            fprintf(out, "Ошибка в строке %d: длина %d, а должна быть %d\n",
                    line_number, len, width);
            errors++;
        } else {
            if (width == no_width) {
                width = len;
            }
            fprintf(out, "%s\n", unpacked);
            rows++;
        }
    }

    if (errors > 0) {
        fprintf(out, "Найдено ошибок: %d\n", errors);
    } else if (rows == 0) {
        fprintf(out, "В файле нет строк карты.\n");
    } else {
        fprintf(out, "Карта корректна: %d строк по %d символов.\n", rows, width);
    }

    fclose(in);
    fclose(out);
    return errors;
}
void print_file(const char *filename)
{
    FILE *file;
    char line[line_size];

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл %s\n", filename);
        return;
    }
    while (fgets(line, line_size, file) != NULL) {
        printf("%s", line);
    }
    fclose(file);
}
void menu_unpack_map(void)
{
    int result;

    result = unpack_map_file(input_file, output_file);
    if (result == map_err_no_input) {
        printf("Ошибка: не найден файл %s!\n", input_file);
    } else if (result == map_err_no_output) {
        printf("Ошибка: не удалось создать файл %s!\n", output_file);
    } else {
        print_file(output_file);
        printf("Результат записан в %s\n", output_file);
    }
}


