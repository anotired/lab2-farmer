#include "adventure.h"
#include <stdio.h>
#include <stdlib.h>

void forest_free(Forest *forest)
{
    int y;
    if (forest->cells != NULL) {
        for (y = 0; y < forest->height; y++) {
            free(forest->cells[y]);
        }
        free(forest->cells);
    }
    forest->cells = NULL;
}

int forest_generate(Forest *forest)
{
    int x;
    int y;
    int camp_x;
    int camp_y;
    forest_free(forest);
    forest->width = forest_min_size + rand() % (forest_max_size - forest_min_size + 1);
    forest->height = forest_min_size + rand() % (forest_max_size - forest_min_size + 1);
    forest->crossings = 0;
    forest->has_portal = 0;
    forest->cells = calloc((size_t)forest->height, sizeof(*forest->cells));
    if (forest->cells == NULL) {
        return 0;
    }
    for (y = 0; y < forest->height; y++) {
        forest->cells[y] = malloc((size_t)forest->width * sizeof(**forest->cells));
        if (forest->cells[y] == NULL) {
            forest_free(forest);
            return 0;
        }
        for (x = 0; x < forest->width; x++) {
            forest->cells[y][x] = rand() % 100 < forest_obstacle_percent
                ? (rand() % 2 == 0 ? forest_tree : forest_rock) : forest_empty;
        }
    }
    forest->x = rand() % forest->width;
    forest->y = rand() % forest->height;
    camp_x = rand() % forest->width;
    camp_y = rand() % forest->height;
    /* Свободные строка и столбец связывают старт с лагерем и краями. */
    for (x = 0; x < forest->width; x++) {
        forest->cells[forest->y][x] = forest_empty;
    }
    for (y = 0; y < forest->height; y++) {
        forest->cells[y][camp_x] = forest_empty;
    }
    forest->cells[camp_y][camp_x] = forest_camp;
    return 1;
}

void forest_draw(const Forest *forest)
{
    int x;
    int y;
    printf("\nЛес %dx%d. Дальность от дома: %d. Переходов через край: %d.\n",
           forest->width, forest->height, forest->distance, forest->crossings);
    for (y = 0; y < forest->height; y++) {
        for (x = 0; x < forest->width; x++) {
            putchar(x == forest->x && y == forest->y ? forest_hero : forest->cells[y][x]);
        }
        putchar('\n');
    }
    puts("@ — герой, T — дерево, R — камень, ^ — лагерь, P — портал.");
}
