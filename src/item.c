#include "game.h"

void handleGold(Tile *t) {
    assert(t->G);
    int g = t->G->gold;

    char *s = (char *) malloc(100 * sizeof(char));
    sprintf(s, "You've picked up %d Golds!", g);
    setTopMsg(s);

    P->gold += g;
    t->G = NULL;    
    t->type = 0;
}

void handleFood(Tile *t) {
    setTopMsg("There's something tasty on the floor");
}

void handleCurse(Tile *t) {
    setTopMsg("Just a curse...");
}

void handleWeapon(Tile *t) {
    setTopMsg("Chopstick on da floor");
}

void discoverItem(Tile *t) {
    int type = t->type;
    if (type != 42) return;
    if (t->G) {
        handleGold(t);
    }
    else if (t->F) {
        handleFood(t);
    }
    else if (t->C) {
        handleCurse(t);
    }
    else if (t->W) {
        handleWeapon(t);
    }
}