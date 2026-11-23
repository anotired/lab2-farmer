#include "player.h"
#include "utils.h"
#include <string.h>

#define diary_file "diary.txt"
#define no_item (-1)

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

