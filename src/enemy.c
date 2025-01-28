#include "game.h"


int isValidTile(Level *L, int x, int y) {
    return x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH && (L->tile[x][y].type == 0 || L->tile[x][y].type == 42);
}

void shortRangeAttack(Level *L, Weapon *W) {
    for (int x = P->x - 1; x <= P->x + 1; ++x) {
        for (int y = P->y - 1; y <= P->y + 1; ++y) {
            //TODO if enemey exists damage it...
        }
    }
    renderMsgAndWait("You Attacked!", 1);
}

void longRangeAttackWithDirection(Level *L, Weapon *W, int dx, int dy) {
    int x = P->x, y = P->y;
    for (int i = 0; i < W->range; ++i, x += dx, y += dy) {
        if (!isValidTile(L, x, y)) return;
        //if hit enemy...
    }
}

void longRangeAttack(Level *L, Weapon *W) {
    while (true) {
        int ch = getch();
        switch (ch) {
        case 'a':
        case 'A':
            longRangeAttackWithDirection(L, W, 0, -1);
            return;
        case 'W':
        case 'w':
            longRangeAttackWithDirection(L, W, -1, 0);
            return;
        case 'd':
        case 'D':
            longRangeAttackWithDirection(L, W, 0, +1);
            return;
        case 'S':
        case 's':
            longRangeAttackWithDirection(L, W, +1, 0);
            return;
        }
        renderMsgAndWait("Choose a correct direction...!", 3);
    }
}

void attack(Level *L) {
    assert(P->weapon[P->def_weapon].td != 0);
    Weapon *W = &P->weapon[P->def_weapon];
    if (W->type == 0 || W->type == 4) shortRangeAttack(L, W);
    else longRangeAttack(L, W);

    P->weapon[P->def_weapon].td--;
    if (P->weapon[P->def_weapon].td == 0) P->def_weapon = 0; // change to Mace
}