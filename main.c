#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define inventory_size 10
#define max_work_hours 24
#define hours_in_day 24
#define max_item_id 9
#define item_empty 0
#define line_size 256
#define max_items 10
#define max_name_len 32
#define items_file "items.txt"
#define diary_file "diary.txt"
#define no_item (-1)
#define input_file "input.txt"
#define output_file "output.txt"
#define map_line_size 512
#define no_width (-1)
#define unpack_error (-1)
#define map_err_no_input (-1)
#define map_err_no_output (-2)
#define menu_last 9

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
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    remove_newline(buffer);
    return 1;
}

int read_int(const char *prompt, int min, int max)
{
    char line[line_size];
    int value;

    while (1) {
        printf("%s (%d-%d): ", prompt, min, max);
        if (!read_line(line, line_size)) {
            printf("\nВвод закрыт. Выход из программы.\n");
            exit(0);
        }
        if (sscanf(line, "%d", &value) != 1) {
            printf("Ошибка: нужно ввести число!\n");
        } else if (value < min || value > max) {
            printf("Ошибка: число должно быть от %d до %d!\n", min, max);
        } else {
            return value;
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

void set_default_names(char item_names[max_items][max_name_len])
{
    strcpy(item_names[0], "Пусто");
    strcpy(item_names[1], "Дерево");
    strcpy(item_names[2], "Камень");
    strcpy(item_names[3], "Семена");
    strcpy(item_names[4], "Кирка");
    strcpy(item_names[5], "Топор");
    strcpy(item_names[6], "Мотыга");
    strcpy(item_names[7], "Броня");
    strcpy(item_names[8], "Верстак");
    strcpy(item_names[9], "Печь");
}

void write_inventory(FILE *out, const int inventory[], char item_names[max_items][max_name_len])
{
    int i;

    for (i = 0; i < inventory_size; i++) {
        fprintf(out, "Слот %d: [%d] - %s\n", i, inventory[i], item_names[inventory[i]]);
    }
}

int load_item_names(char item_names[max_items][max_name_len], const char *filename)
{
    FILE *file;
    char line[line_size];
    int count = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    while (count < max_items && fgets(line, line_size, file) != NULL) {
        remove_newline(line);
        if (strlen(line) > 0) {
            strncpy(item_names[count], line, max_name_len - 1);
            item_names[count][max_name_len - 1] = '\0';
        }
        count++;
    }

    fclose(file);
    return count;
}

void put_item(int inventory[], int slot, int item_id)
{
    inventory[slot] = item_id;
}

void drop_item(int inventory[], int slot)
{
    inventory[slot] = item_empty;
}

void remove_duplicates(int inventory[])
{
    int i;
    int j;

    for (i = 0; i < inventory_size; i++) {
        if (inventory[i] != item_empty) {
            for (j = i + 1; j < inventory_size; j++) {
                if (inventory[j] == inventory[i]) {
                    inventory[j] = item_empty;
                }
            }
        }
    }
}

int find_item_id(char item_names[max_items][max_name_len], const char *name)
{
    int id;

    for (id = 0; id < max_items; id++) {
        if (strcmp(item_names[id], name) == 0) {
            return id;
        }
    }
    return no_item;
}

int find_slots(const int inventory[], int item_id, int slots[])
{
    int i;
    int count = 0;

    for (i = 0; i < inventory_size; i++) {
        if (inventory[i] == item_id) {
            slots[count] = i;
            count++;
        }
    }
    return count;
}

void write_diary_entry(FILE *out, const char *farmer_name, int day, int hour,
                       const int inventory[], char item_names[max_items][max_name_len])
{
    fprintf(out, "===== Запись дневника =====\n");
    fprintf(out, "Фермер: %s\n", farmer_name);
    fprintf(out, "Время: День %d, %02d:00\n", day, hour);
    fprintf(out, "Инвентарь:\n");
    write_inventory(out, inventory, item_names);
    fprintf(out, "\n");
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
void print_menu(const char *farmer_name)
{
    printf("\n===== Весёлый фермер. Привет, %s! =====\n", farmer_name);
    printf("[0] Выход\n");
    printf("[1] Посмотреть на часы\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет\n");
    printf("[6] Устранить дубликаты\n");
    printf("[7] Найти предмет по названию\n");
    printf("[8] Записать состояние в дневник\n");
    printf("[9] Расшифровать карту (input.txt)\n");
}

void menu_search_item(const int inventory[], char item_names[max_items][max_name_len])
{
    char name[max_name_len];
    int slots[inventory_size];
    int item_id;
    int count;
    int i;

    read_text("Введите название предмета: ", name, max_name_len);

    item_id = find_item_id(item_names, name);
    if (item_id == no_item) {
        printf("Предмета \"%s\" нет в каталоге.\n", name);
        return;
    }

    count = find_slots(inventory, item_id, slots);
    if (count == 0) {
        printf("Предмет \"%s\" (ID %d) есть в каталоге, но в рюкзаке его нет.\n", name, item_id);
        return;
    }

    printf("Предмет \"%s\" (ID %d) лежит в слотах:", name, item_id);
    for (i = 0; i < count; i++) {
        printf(" %d", slots[i]);
    }
    printf("\n");
}

void menu_diary(const char *farmer_name, int day, int hour,
                const int inventory[], char item_names[max_items][max_name_len])
{
    FILE *file;

    file = fopen(diary_file, "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл %s для записи!\n", diary_file);
        return;
    }

    write_diary_entry(stdout, farmer_name, day, hour, inventory, item_names);
    write_diary_entry(file, farmer_name, day, hour, inventory, item_names);

    fclose(file);
    printf("Запись добавлена в %s\n", diary_file);
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


int main(void)
{
    int current_day = 1;
    int current_hour = 8;
    int inventory[inventory_size] = {1, 2, 3, 4, 8, 2, 5, 7, 8, 1};
    /* 0 - пусто, 1 - дерево, 2 - камень, 3 - семена, 4 - кирка, 5 - топор, 6 - мотыга,
    7 - броня, 8 - верстак, 9 - печь */
    char item_names[max_items][max_name_len];
    char farmer_name[max_name_len];
    int loaded;
    int choice;
    int hours;
    int slot;
    int item_id;

    set_default_names(item_names);
    loaded = load_item_names(item_names, items_file);
    if (loaded == 0) {
        printf("Предупреждение: файл %s не найден. Используются стандартные названия.\n", items_file);
    } else if (loaded < max_items) {
        printf("Предупреждение: в %s только %d строк из %d. Остальные названия стандартные.\n",
               items_file, loaded, max_items);
    }
    read_text("Введите имя фермера: ", farmer_name, max_name_len);

    while (1) {
        print_menu(farmer_name);
        choice = read_int("Введите действие", 0, menu_last);

        switch (choice) {
            case 0:
                printf("Вы вышли из игры\n");
                return 0;

            case 1:
                print_time(current_day, current_hour);
                break;

            case 2:
                hours = read_int("Сколько часов работать", 0, max_work_hours);
                current_day = calc_day(current_day, current_hour, hours);
                current_hour = calc_hour(current_hour, hours);
                printf("Вы поработали %d ч.\n", hours);
                print_time(current_day, current_hour);
                break;

            case 3:
                write_inventory(stdout, inventory, item_names);
                break;

            case 4:
                slot = read_int("Номер слота", 0, inventory_size - 1);
                item_id = read_int("ID предмета", 0, max_item_id);
                put_item(inventory, slot, item_id);
                printf("В слот %d положен предмет: %s\n", slot, item_names[item_id]);
                break;

            case 5:
                slot = read_int("Номер слота", 0, inventory_size - 1);
                if (inventory[slot] == item_empty) {
                    printf("Слот %d уже пуст.\n", slot);
                } else {
                    printf("Выброшен предмет: %s\n", item_names[inventory[slot]]);
                    drop_item(inventory, slot);
                }
                break;

            case 6:
                printf("Инвентарь ДО:\n");
                write_inventory(stdout, inventory, item_names);
                remove_duplicates(inventory);
                printf("Инвентарь ПОСЛЕ:\n");
                write_inventory(stdout, inventory, item_names);
                break;

            case 7:
                menu_search_item(inventory, item_names);
                break;

            case 8:
                menu_diary(farmer_name, current_day, current_hour, inventory, item_names);
                break;

            case 9:
                menu_unpack_map();
                break;

            default:
                printf("Такого пункта нет в меню!\n");
                break;
        }
    }
}