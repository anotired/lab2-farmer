#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>

#define inventory_size 10
#define max_item_id 9
#define item_empty 0
#define max_items 10
#define max_name_len 32
#define items_file "items.txt"

typedef struct Player {
    char name[max_name_len];
    int hp, max_hp;
    int stamina, max_stamina;
    int gold;
    int x, y;
    int inventory[inventory_size];
} Player;

Player *player_create(void);
void player_destroy(Player *player);
void player_card(const Player *player, int day, int hour,
                 char item_names[max_items][max_name_len]);
void menu_put_item(Player *player, char item_names[max_items][max_name_len]);
void menu_drop_item(Player *player, char item_names[max_items][max_name_len]);
void menu_remove_duplicates(Player *player, char item_names[max_items][max_name_len]);
void set_default_names(char item_names[max_items][max_name_len]);
void write_inventory(FILE *out, const int inventory[], char item_names[max_items][max_name_len]);
int load_item_names(char item_names[max_items][max_name_len], const char *filename);



void menu_search_item(const Player *player, char item_names[max_items][max_name_len]);
void menu_diary(const Player *player, int day, int hour, char item_names[max_items][max_name_len]);

#endif
