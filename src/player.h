#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>

#define inventory_size 10
#define max_item_id 9
#define item_empty 0
#define max_items 10
#define max_name_len 32
#define items_file "items.txt"

void set_default_names(char item_names[max_items][max_name_len]);
void write_inventory(FILE *out, const int inventory[], char item_names[max_items][max_name_len]);
int load_item_names(char item_names[max_items][max_name_len], const char *filename);
void put_item(int inventory[], int slot, int item_id);
void drop_item(int inventory[], int slot);
void remove_duplicates(int inventory[]);
int find_item_id(char item_names[max_items][max_name_len], const char *name);
int find_slots(const int inventory[], int item_id, int slots[]);
void write_diary_entry(FILE *out, const char *farmer_name, int day, int hour,
                       const int inventory[], char item_names[max_items][max_name_len]);
void menu_search_item(const int inventory[], char item_names[max_items][max_name_len]);
void menu_diary(const char *farmer_name, int day, int hour,
                const int inventory[], char item_names[max_items][max_name_len]);

#endif
