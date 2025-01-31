#include "game.h"

void saveGame() {
    FILE *F = fopen(username, "wb");
    fwrite(P, sizeof(P), 1, F);
    for (int i = 0; i < MAX_FOOD; ++i) fwrite(P->food[i], sizeof(Food), 1, F);
    for (int i = 0; i < MAX_CURSE; ++i) fwrite(P->curse[i], sizeof(Curse), 1, F);
    fwrite(&game_movement_timer, sizeof(int), 1, F);
    fwrite(&GOD_MODE, sizeof(int), 1, F);
    fwrite(&DIFF_LEVEL, sizeof(int), 1, F);
    fwrite(&MAIN_COLOR, sizeof(int), 1, F);
    fwrite(&toggle_map_status, sizeof(int), 1, F);

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
}

void loadGame() {

}