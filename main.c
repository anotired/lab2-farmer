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
#define menu_last 6
 
void remove_newline(char *text)
{
    int len = strlen(text);
 
    while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r')) {
        text[len - 1] = '\0';
        len--;
    }
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
    int choice;
    int hours;
    int slot;
    int item_id;
 
    set_default_names(item_names);
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
 
            default:
                printf("Такого пункта нет в меню!\n");
                break;
        }
    }
}
 