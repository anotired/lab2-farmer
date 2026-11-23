#include "player.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

#define diary_file "diary.txt"
#define no_item (-1)
#define START_HP 100
#define START_STAMINA 50

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

static void remove_duplicates(int inventory[])
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

static int find_item_id(char item_names[max_items][max_name_len], const char *name)
{
    int id;

    for (id = 0; id < max_items; id++) {
        if (strcmp(item_names[id], name) == 0) {
            return id;
        }
    }
    return no_item;
}

static int find_slots(const int inventory[], int item_id, int slots[])
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

static void write_diary_entry(FILE *out, const char *farmer_name, int day, int hour,
                       const int inventory[], char item_names[max_items][max_name_len])
{
    fprintf(out, "===== Запись дневника =====\n");
    fprintf(out, "Фермер: %s\n", farmer_name);
    fprintf(out, "Время: День %d, %02d:00\n", day, hour);
    fprintf(out, "Инвентарь:\n");
    write_inventory(out, inventory, item_names);
    fprintf(out, "\n");
}

void menu_search_item(const Player *player, char item_names[max_items][max_name_len])
{
    char name[max_name_len];
    int slots[inventory_size];
    int item_id;
    int count;
    int i;

    if (!read_text("Введите название предмета: ", name, max_name_len)) {
        return;
    }

    item_id = find_item_id(item_names, name);
    if (item_id == no_item) {
        printf("Предмета \"%s\" нет в каталоге.\n", name);
        return;
    }

    count = find_slots(player->inventory, item_id, slots);
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

void menu_diary(const Player *player, int day, int hour,
                char item_names[max_items][max_name_len])
{
    FILE *file;

    file = fopen(diary_file, "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл %s для записи!\n", diary_file);
        return;
    }

    write_diary_entry(stdout, player->name, day, hour, player->inventory, item_names);
    write_diary_entry(file, player->name, day, hour, player->inventory, item_names);

    fclose(file);
    printf("Запись добавлена в %s\n", diary_file);
}


Player *player_create(void)
{
    Player initial = {
        .hp = START_HP, .max_hp = START_HP,
        .stamina = START_STAMINA, .max_stamina = START_STAMINA,
        .inventory = {1, 2, 3, 4, 8, 2, 5, 7, 8, 1}
    };
    if (!read_text("Введите имя фермера: ", initial.name, sizeof(initial.name))) {
        return NULL;
    }
    Player *player = malloc(sizeof(*player));
    if (player == NULL) {
        puts("Не удалось выделить память для персонажа.");
        return NULL;
    }
    *player = initial;
    return player;
}

void player_destroy(Player *player)
{
    if (player != NULL) {
        printf("Память структуры по адресу %p освобождена\n", (void *)player);
        free(player);
    }
}

void player_card(const Player *player, int day, int hour,
                 char item_names[max_items][max_name_len])
{
    printf("\n===== Карточка: %s =====\n", player->name);
    printf("HP: %d/%d | Выносливость: %d/%d | Золото: %d\n",
           player->hp, player->max_hp, player->stamina, player->max_stamina, player->gold);
    print_time(day, hour);
    write_inventory(stdout, player->inventory, item_names);
}

void menu_put_item(Player *player, char item_names[max_items][max_name_len])
{
    int slot, id;
    if (read_int("Номер слота", 0, inventory_size - 1, &slot) &&
        read_int("ID предмета", 0, max_item_id, &id)) {
        player->inventory[slot] = id;
        printf("В слот %d положен предмет: %s\n", slot, item_names[id]);
    }
}

void menu_drop_item(Player *player, char item_names[max_items][max_name_len])
{
    int slot;
    if (read_int("Номер слота", 0, inventory_size - 1, &slot)) {
        int id = player->inventory[slot];
        if (id == item_empty) {
            printf("Слот %d уже пуст.\n", slot);
        } else {
            printf("Выброшен предмет: %s\n", item_names[id]);
            player->inventory[slot] = item_empty;
        }
    }
}

void menu_remove_duplicates(Player *player, char item_names[max_items][max_name_len])
{
    puts("Инвентарь ДО:");
    write_inventory(stdout, player->inventory, item_names);
    remove_duplicates(player->inventory);
    puts("Инвентарь ПОСЛЕ:");
    write_inventory(stdout, player->inventory, item_names);
}
