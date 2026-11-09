#include "../src/adventure.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static void test_generation(void)
{
    Forest forest = {0};
    int iteration;
    for (iteration = 0; iteration < 500; iteration++) {
        int x;
        int y;
        int camps = 0;
        assert(forest_generate(&forest));
        assert(forest.width >= forest_min_size && forest.width <= forest_max_size);
        assert(forest.height >= forest_min_size && forest.height <= forest_max_size);
        assert(forest.cells[forest.y][forest.x] == forest_empty ||
               forest.cells[forest.y][forest.x] == forest_camp);
        for (y = 0; y < forest.height; y++) {
            for (x = 0; x < forest.width; x++) {
                char tile = forest.cells[y][x];
                assert(tile == forest_empty || tile == forest_camp ||
                       tile == forest_tree || tile == forest_rock);
                camps += tile == forest_camp;
            }
        }
        assert(camps == 1);
        assert(forest.crossings == 0 && !forest.has_portal);
    }
    forest_free(&forest);
    forest_free(&forest);
    assert(forest.cells == NULL);
}

static void test_ring_and_portal(void)
{
    Forest forest = {0};
    char **original;
    int x;
    int y;
    int portal_x = -1;
    int portal_y = -1;
    int portals = 0;
    assert(forest_generate(&forest));
    for (y = 0; y < forest.height; y++) {
        for (x = 0; x < forest.width; x++) {
            forest.cells[y][x] = forest_empty;
        }
    }
    forest.x = 0;
    forest.y = 0;
    forest.cells[1][1] = forest_camp;
    original = forest.cells;
    forest.cells[0][forest.width - 1] = forest_tree;
    assert(forest_move(&forest, -1, 0) == 0);
    assert(forest.x == 0 && forest.y == 0 && forest.crossings == 0);
    forest.cells[0][forest.width - 1] = forest_empty;
    assert(forest_move(&forest, -1, 0) == 1);
    assert(forest.x == forest.width - 1 && forest.crossings == 1);
    assert(!forest.has_portal && forest.cells == original);
    assert(forest_move(&forest, 1, 0) == 1);
    assert(forest.x == 0 && forest.crossings == 2 && !forest.has_portal);
    assert(forest_move(&forest, 0, -1) == 1);
    assert(forest.y == forest.height - 1 && forest.crossings == 3);
    assert(forest.has_portal && forest.cells == original && forest.distance == 0);
    for (y = 0; y < forest.height; y++) {
        for (x = 0; x < forest.width; x++) {
            if (forest.cells[y][x] == forest_portal) {
                portal_x = x;
                portal_y = y;
                portals++;
            }
        }
    }
    assert(portals == 1);
    assert(portal_x != forest.x || portal_y != forest.y);
    assert(forest_move(&forest, 0, 1) == 1);
    assert(forest.y == 0 && forest.crossings == 4);
    forest.x = (portal_x + forest.width - 1) % forest.width;
    forest.y = portal_y;
    assert(forest_move(&forest, 1, 0) == 1);
    assert(forest.distance == 1);
    assert(forest.crossings == 0 && !forest.has_portal);
    forest_free(&forest);
}

int main(void)
{
    srand(5);
    test_generation();
    test_ring_and_portal();
    puts("Проверки генерации, коллизий, кольца и портала пройдены.");
    return 0;
}
