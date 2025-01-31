#include "game.h"

void saveGame() {
    FILE *F = fopen(username, "wb");
    fwrite(P, sizeof(P), 1, F);
    fwrite(&game_movement_timer, sizeof(int), 1, F);
    fwrite(&GOD_MODE, sizeof(int), 1, F);
    fwrite(&DIFF_LEVEL, sizeof(int), 1, F);
    fwrite(&MAIN_COLOR, sizeof(int), 1, F);
    fwrite(&toggle_map_status, sizeof(int), 1, F);

    int num_food = 0, num_curse = 0;
    for (int i = 0; i < MAX_FOOD; ++i) num_food += P->food[i] != NULL;
    for (int i = 0; i < MAX_CURSE; ++i) num_food += P->curse[i] != NULL;

    fwrite(&num_food, sizeof(int), 1, F);
    fwrite(&num_curse, sizeof(int), 1, F);

    for (int i = 0; i < MAX_FOOD; ++i) if (P->food[i] != NULL) fwrite(P->food[i], sizeof(Food), 1, F);
    for (int i = 0; i < MAX_CURSE; ++i) if (P->curse[i] != NULL) fwrite(P->curse[i], sizeof(Curse), 1, F);

    fwrite(G, sizeof(G), 1, F);
    for (int i = 0; i < G->num_level; ++i) {
        for (int x = 0; x < HEIGHT; ++x) {
            for (int y = 0; y < WIDTH; ++y) {
                Tile *T= &G->lvl[i].tile[x][y];
                if (T->type == 42)
                {
                    int t = 0;
                    if (T->W)
                        t = 1;
                    else if (T->C)
                        t = 2;
                    else if (T->G)
                        t = 3;
                    else if (T->F)
                        t = 4;
                    fwrite(&t, sizeof(int), 1, F);
                    if (t == 1)
                        fwrite(T->W, sizeof(Weapon), 1, F);
                    else if (t == 2)
                        fwrite(T->C, sizeof(Curse), 1, F);
                    else if (t == 3)
                        fwrite(T->G, sizeof(Gold), 1, F);
                    else if (t == 4)
                        fwrite(T->F, sizeof(Food), 1, F);
                }
                if (T->type == 16 || T->type == 18 || T->type == 20) { //LOCK TODO
                    fwrite(T->lock, sizeof(Lock), 1, F);
                }
            }
        }
    }
    fclose(F);
}

void resumeGame() {
    FILE *F = fopen(username, "rb");
    if (!F) {
        renderMsgAndWait("Error: Unable to load game file.\n", 3);
        return;
    }

    fread(P, sizeof(P), 1, F);
    fread(&game_movement_timer, sizeof(int), 1, F);
    fread(&GOD_MODE, sizeof(int), 1, F);
    fread(&DIFF_LEVEL, sizeof(int), 1, F);
    fread(&MAIN_COLOR, sizeof(int), 1, F);
    fread(&toggle_map_status, sizeof(int), 1, F);

    int num_food, num_curse;
    fread(&num_food, sizeof(int), 1, F);
    fread(&num_curse, sizeof(int), 1, F);

    for (int i = 0; i < num_food; ++i) {
        P->food[i] = (Food *)malloc(sizeof(Food));
        fread(P->food[i], sizeof(Food), 1, F);
    }

    for (int i = 0; i < num_curse; ++i) {
        P->curse[i] = (Curse *)malloc(sizeof(Curse));
        fread(P->curse[i], sizeof(Curse), 1, F);
    }

    fread(G, sizeof(G), 1, F);
    for (int i = 0; i < G->num_level; ++i) {
        for (int x = 0; x < HEIGHT; ++x) {
            for (int y = 0; y < WIDTH; ++y) {
                Tile *T = &G->lvl[i].tile[x][y];
                if (T->type == 42) {
                    int t;
                    fread(&t, sizeof(int), 1, F);
                    if (t == 1) {
                        T->W = (Weapon *)malloc(sizeof(Weapon));
                        fread(T->W, sizeof(Weapon), 1, F);
                    } else if (t == 2) {
                        T->C = (Curse *)malloc(sizeof(Curse));
                        fread(T->C, sizeof(Curse), 1, F);
                    } else if (t == 3) {
                        T->G = (Gold *)malloc(sizeof(Gold));
                        fread(T->G, sizeof(Gold), 1, F);
                    } else if (t == 4) {
                        T->F = (Food *)malloc(sizeof(Food));
                        fread(T->F, sizeof(Food), 1, F);
                    }
                }
                if (T->type == 16 || T->type == 18 || T->type == 20) { // Locks
                    T->lock = (Lock *)malloc(sizeof(Lock));
                    fread(T->lock, sizeof(Lock), 1, F);
                }
            }
        }
    }
    fclose(F);
    gameloop();
}