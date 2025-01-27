#include "game.h"

const char* NO_ITEM_ERROR[] = {
        "Looks like you just summoned the invisible item! Truly a master of nothingness.",
        "Congratulations! You've successfully picked up... absolutely nothing. A true champion of the void!",
        "Nice try! But even the ghosts didn’t leave anything behind for you to pick up.",
        "Your talent for finding air is nothing short of astonishing! Are you sure you want to carry that ‘weight’?",
        "If picking up nothing was an Olympic sport, you’d definitely take home the gold!",
        "You’ve got the magic touch! Too bad that magic is only good for picking up thin air.",
        "Ah, the classic ‘attempted pick-up’ maneuver! It seems gravity is working against you today.",
        "Well done! You’ve shown impressive dedication to the art of empty-handedness.",
        "You should really check your pockets. They must be full of all the things you can't pick up!",
        "Looks like you just earned the prestigious 'Master of the Empty Hand' award! Wear it with pride!"
    };
#define NO_ITEM_ERROR_SZ 10
char *WEAPON_NAME_BY_TYPE[] = {"Mace", "Dagger", "Magic Wand", "Normal Arrow", "Sword"};


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
    setTopMsg("A weapon ? Maybe useful!");
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

void pickUpWeapon(Tile *t) {
    assert(t->W);
    P->weapon[t->W->type].td += t->W->td;

    char *s = (char *)malloc(100 * sizeof(char));
    sprintf(s, "Hmm... A %s Picked up!", WEAPON_NAME_BY_TYPE[t->W->type]);
    setTopMsg(s);

    t->W = NULL;
    t->type = 0;
}

//Todo (MORE ITEMS)
void searchItem(Tile *t) {
    int type = t->type;
    if (type != 42) {
        setTopMsg(NO_ITEM_ERROR[rand() % NO_ITEM_ERROR_SZ]);
        return;
    }
    if (t->G) assert(false);
    if (t->F) {
        //TODO add Food
        renderMsgAndWait("Picking up da Food", 1);
    }
    else if (t->C) {
        renderMsgAndWait("Picking up da Curse", 1);
    }
    else if (t->W) {
       pickUpWeapon(t);
    }
}

void showWeapon() {
    char *menu[MAX_WEAPON], *msg[MAX_WEAPON];
    int sz = 0;
    for (int i = 0; i < MAX_WEAPON; ++i) {
        if (P->weapon[i].td) {
            menu[sz] = WEAPON_NAME_BY_TYPE[P->weapon[i].type];

            if (P->weapon[i].td < 0) {
                msg[sz] = "Inifinty Times :)";
            }
            else {
                char *s = (char *)malloc(100 * sizeof(char));
                sprintf(s, "You have %d of it", P->weapon[i].td);
                msg[sz] = s;
            }
            ++sz;
        }
    }
    P->def_weapon = createMenu(menu, msg, sz);
}