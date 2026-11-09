#ifndef ADVENTURE_H
#define ADVENTURE_H

#define forest_min_size 5
#define forest_max_size 30
#define forest_obstacle_percent 20
#define forest_portal_crossings 3
#define forest_empty '.'
#define forest_tree 'T'
#define forest_rock 'R'
#define forest_camp '^'
#define forest_portal 'P'
#define forest_hero '@'

typedef struct {
    char **cells;
    int width;
    int height;
    int x;
    int y;
    int distance;
    int crossings;
    int has_portal;
} Forest;

void forest_free(Forest *forest);
int forest_generate(Forest *forest);
void forest_draw(const Forest *forest);
int forest_move(Forest *forest, int dx, int dy);
void adventure(void);

#endif
