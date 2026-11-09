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

static int forest_place_portal(Forest *forest)
{
    int total = forest->width * forest->height;
    int *queue = malloc((size_t)total * sizeof(*queue));
    unsigned char *seen = calloc((size_t)total, sizeof(*seen));
    const int dx[] = {0, -1, 0, 1};
    const int dy[] = {-1, 0, 1, 0};
    int head = 0;
    int tail = 0;
    int selected = -1;
    int count = 0;
    int i;
    if (queue == NULL || seen == NULL) {
        free(queue);
        free(seen);
        return 0;
    }
    queue[tail++] = forest->y * forest->width + forest->x;
    seen[queue[0]] = 1;
    while (head < tail) {
        int cell = queue[head++];
        int x = cell % forest->width;
        int y = cell / forest->width;
        if (forest->cells[y][x] == forest_empty &&
            (x != forest->x || y != forest->y)) {
            count++;
            if (rand() % count == 0) {
                selected = cell;
            }
        }
        for (i = 0; i < (int)(sizeof(dx) / sizeof(dx[0])); i++) {
            int nx = (x + dx[i] + forest->width) % forest->width;
            int ny = (y + dy[i] + forest->height) % forest->height;
            int next = ny * forest->width + nx;
            char tile = forest->cells[ny][nx];
            if (!seen[next] && tile != forest_tree && tile != forest_rock) {
                seen[next] = 1;
                queue[tail++] = next;
            }
        }
    }
    free(queue);
    free(seen);
    if (selected < 0) {
        return 0;
    }
    forest->cells[selected / forest->width][selected % forest->width] = forest_portal;
    forest->has_portal = 1;
    puts("Вы заблудились! В лесу появился портал P.");
    return 1;
}

int forest_move(Forest *forest, int dx, int dy)
{
    int x = forest->x + dx;
    int y = forest->y + dy;
    int crossed = x < 0 || x >= forest->width || y < 0 || y >= forest->height;
    x = (x + forest->width) % forest->width;
    y = (y + forest->height) % forest->height;
    if (forest->cells[y][x] == forest_tree || forest->cells[y][x] == forest_rock) {
        puts("Путь закрыт препятствием.");
        return 0;
    }
    forest->x = x;
    forest->y = y;
    if (crossed) {
        /* После появления портала счётчик насыщается, исключая переполнение. */
        if (forest->crossings < forest_portal_crossings) {
            forest->crossings++;
        }
        puts("Вы пересекли край и оказались на другой стороне того же леса.");
    }
    if (forest->cells[y][x] == forest_portal) {
        forest->distance++;
        if (!forest_generate(forest)) {
            return -1;
        }
        puts("Портал перенёс вас на новый участок леса.");
        return 1;
    }
    if (forest->crossings >= forest_portal_crossings && !forest->has_portal) {
        if (!forest_place_portal(forest)) {
            return -1;
        }
    }
    return 1;
}
