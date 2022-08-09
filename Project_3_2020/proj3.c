#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define baseDown(r, c) ((r + c) & 1)

enum direction{L = 1, R = 2, UD = 4};

typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

bool isborder(Map *map, int r, int c, int border)
{
    return map->cells[map->cols*(r-1)+c-1] & border;
}

bool loadMap(Map *map, char *file)
{
    FILE *fp = fopen(file, "r");
    if (!fp)
        return false;

    if (fscanf(fp, "%d %d", &map->rows, &map->cols) != 2) {
        fclose(fp);
        return false;
    }

    map->cells = malloc(map->rows * map->cols);

    if (!map->cells) {
        fclose(fp);
        return false;
    }

    for (int i = 0; i < map->rows * map->cols; ++i)
        if (fscanf(fp, "%hhu", &map->cells[i]) != 1 || map->cells[i] > 7) {
            fclose(fp);
            return false;
        }

    fclose(fp);
    return true;
}

int test(char *file, Map *map)
{
    if (!loadMap(map, file))
        return 1;

    for (int i = 1; i <= map->rows; ++i)
        for (int j = 1; j < map->cols; ++j)
            if (isborder(map, i, j, R) != isborder(map, i, j+1, L))
                return 1;

    for (int i = 1; i < map->rows; ++i)
        for (int j = 1 + (i & 1); j <= map->cols; j += 2)
            if (isborder(map, i, j, UD) != isborder(map, i+1, j, UD))
                return 1;

    return 0;
}
//Function to decide where to go next based on leftright, last move and actual cell - optimized version of commented version under main
int whereTo(Map *map, int from, int leftright, int r, int c)
{
    while (1) {
        if (from == L)
            from = (leftright == baseDown(r, c) ? R : UD);
        else if (from == R)
            from = (leftright == baseDown(r, c) ? UD : L);
        else if (from == UD)
            from = (leftright == baseDown(r, c) ? L : R);

        if (!isborder(map, r, c, from))
            return from;
    }
}

int start_border(Map *map, int r, int c, int leftright)
{
    if (c == 1 && !isborder(map, r, c, L))
        return whereTo(map, L, leftright, r, c);
    if (c == map->cols && !isborder(map, r, c, L))
        return whereTo(map, R, leftright, r, c);
    if (((r == 1 && !baseDown(r, c)) || (r == map->rows && baseDown(r, c))) && !isborder(map, r, c, UD))
        return whereTo(map, UD, leftright, r, c);
    return 0;
}

bool move(Map map, int *where, int *r, int *c, int leftright)
{
    if (*where == L) {
        printf("%d, %d\n", *r, (*c)--);
        *where = whereTo(&map, R, leftright, *r, *c);
    } else if (*where == R) {
        printf("%d, %d\n", *r, (*c)++);
        *where = whereTo(&map, L, leftright, *r, *c);
    } else if (*where == UD) {
        if (baseDown(*r, *c))
            printf("%d, %d\n", (*r)++, *c);
        else
            printf("%d, %d\n", (*r)--, *c);
        *where = whereTo(&map, UD, leftright, *r, *c);
    } else {
        fprintf(stderr, "Can't move.\n");
        return true;
    }
    if (*r < 1 || *c < 1 || *r > map.rows || *c > map.cols)
        return true;

    return false;
}

int errfree(Map *map)
{
    fprintf(stderr, "Error: Invalid arguments\n");
    free(map->cells);
    return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    Map map = {0};

    if (argc == 2 && !strcmp("--help", argv[1])) {
        printf("Napoveda, kterou se mi nechce psat\n");
    } else if (argc == 3 && !strcmp("--test", argv[1])) {
        if (test(argv[2], &map))
            printf("Invalid.\n");
        else
            printf("Valid.\n");
    } else if (argc == 5) {
        if (!loadMap(&map, argv[4]))
            return errfree(&map);

        char *endptr;
        int r = strtol(argv[2], &endptr, 10);
        if (*endptr || r < 1 || r > map.rows || !argv[2][0])
            return errfree(&map);

        int c = strtol(argv[3], &endptr, 10);
        if (*endptr || c < 1 || c > map.cols || !argv[3][0])
            return errfree(&map);

        if (!strcmp("--lpath", argv[1])) {
            int where = start_border(&map, r, c, 1);
            while (!move(map, &where, &r, &c, 1)) {} ;
        } else if (!strcmp("--rpath", argv[1])) {
            int where = start_border(&map, r, c, 0);
            while (!move(map, &where, &r, &c, 0)) {} ;
        } else
            return errfree(&map);
    } else {
        return errfree(&map);
    }
    free(map.cells);
    return EXIT_SUCCESS;
}

/*int whereTo(Map *map, int from, int leftright, int r, int c)
{
    bool downbase = baseDown(r, c);

    while (1) {
        if (from == L && leftright && downbase)
            from = R;
        else if (from == R && leftright && downbase)
            from = UD;
        else if (from == UD && leftright && downbase)
            from = L;
        else if (from == L && leftright && !downbase)
            from = UD;
        else if (from == R && leftright && !downbase)
            from = L;
        else if (from == UD && leftright && !downbase)
            from = R;
        else if (from == L && !leftright && downbase)
            from = UD;
        else if (from == R && !leftright && downbase)
            from = L;
        else if (from == UD && !leftright && downbase)
            from = R;
        else if (from == L && !leftright && !downbase)
            from = R;
        else if (from == R && !leftright && !downbase)
            from = UD;
        else if (from == UD && !leftright && !downbase)
            from = L;

        if (!isborder(map, r, c, from))
            return from;
    }
}*/
