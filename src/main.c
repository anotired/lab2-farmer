#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "player.h"
#include "utils.h"

#define max_work_hours 24
typedef enum {
    MENU_EXIT, MENU_CREATE, MENU_CARD, MENU_INVENTORY, MENU_PUT, MENU_DROP,
    MENU_DEDUPLICATE, MENU_SEARCH, MENU_DIARY, MENU_UNPACK, MENU_ADVENTURE,
    MENU_CLOCK, MENU_WORK
} MenuAction;

static void print_menu(const Player *player)
{
    puts("\n===== Весёлый фермер =====");
    if (player != NULL) {
        printf("Привет, %s!\n", player->name);
    }
    puts("[0] Выход\n[1] Создать / пересоздать персонажа\n"
         "[2] Карточка персонажа\n[3] Посмотреть инвентарь\n"
         "[4] Положить предмет в слот\n[5] Выбросить предмет\n"
         "[6] Устранить дубликаты\n[7] Найти предмет по названию\n"
         "[8] Записать состояние в дневник\n[9] Расшифровать карту (input.txt)\n"
         "[10] Отправиться в приключение\n[11] Посмотреть на часы\n"
         "[12] Промотать время (Поработать)");
}

static void recreate_player(Player **player)
{
    int confirm;
    if (*player != NULL &&
        (!read_int("Удалить старого персонажа? 0 — нет, 1 — да", 0, 1, &confirm) ||
         !confirm)) {
        return;
    }
    Player *replacement = player_create();
    if (replacement != NULL) {
        player_destroy(*player);
        *player = replacement;
    }
}

static int needs_player(MenuAction action)
{
    return (action >= MENU_CARD && action <= MENU_DIARY) || action == MENU_ADVENTURE;
}

static void dispatch(MenuAction action, Player **player, int *day, int *hour,
                     char item_names[max_items][max_name_len])
{
    Player *hero = *player;
    int hours;
    if (hero == NULL && needs_player(action)) {
        puts("Сначала создайте персонажа в пункте [1].");
        return;
    }
    switch (action) {
        case MENU_CREATE: recreate_player(player); break;
        case MENU_CARD: player_card(hero, *day, *hour, item_names); break;
        case MENU_INVENTORY: write_inventory(stdout, hero->inventory, item_names); break;
        case MENU_PUT: menu_put_item(hero, item_names); break;
        case MENU_DROP: menu_drop_item(hero, item_names); break;
        case MENU_DEDUPLICATE: menu_remove_duplicates(hero, item_names); break;
        case MENU_SEARCH: menu_search_item(hero, item_names); break;
        case MENU_DIARY: menu_diary(hero, *day, *hour, item_names); break;
        case MENU_UNPACK: menu_unpack_map(); break;
        case MENU_ADVENTURE: adventure(); break;
        case MENU_CLOCK: print_time(*day, *hour); break;
        case MENU_WORK:
            if (read_int("Сколько часов работать", 0, max_work_hours, &hours)) {
                *day = calc_day(*day, *hour, hours);
                *hour = calc_hour(*hour, hours);
                print_time(*day, *hour);
            }
            break;
        case MENU_EXIT: break;
    }
}

int main(void)
{
    int day = 1, hour = 8;
    Player *player = NULL;
    char item_names[max_items][max_name_len];
    int choice;

    srand((unsigned int)time(NULL));
    set_default_names(item_names);
    int loaded = load_item_names(item_names, items_file);
    if (loaded < max_items) {
        printf("Предупреждение: загружено %d из %d названий из %s. "
               "Для остальных используются стандартные названия.\n",
               loaded, max_items, items_file);
    }
    while (!feof(stdin) && !ferror(stdin)) {
        print_menu(player);
        if (!read_int("Введите действие", MENU_EXIT, MENU_WORK, &choice) ||
            choice == MENU_EXIT) {
            break;
        }
        dispatch((MenuAction)choice, &player, &day, &hour, item_names);
    }
    player_destroy(player);
    puts("Вы вышли из игры");
    return 0;
}
