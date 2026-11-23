#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "player.h"
#include "utils.h"

#define max_work_hours 24
#define menu_last 10

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
    printf("[10] Отправиться в приключение\n");
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

    srand((unsigned int)time(NULL));
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

            case 10:
                adventure();
                break;

            default:
                printf("Такого пункта нет в меню!\n");
                break;
        }
    }
}