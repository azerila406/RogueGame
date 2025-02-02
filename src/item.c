#include "game.h"

const char* NO_ITEM_ERROR[] = {
        "Looks like you just summoned the invisible item! Truly a master of nothingness.",
        "Congratulations! You've successfully picked up... absolutely nothing. A true champion of the void!",
        "Nice try! But even the ghosts didn't leave anything behind for you to pick up.",
        "Your talent for finding air is nothing short of astonishing! Are you sure you want to carry that ‘weight'?",
        "If picking up nothing was an Olympic sport, you'd definitely take home the gold!",
        "You've got the magic touch! Too bad that magic is only good for picking up thin air.",
        "Ah, the classic ‘attempted pick-up' maneuver! It seems gravity is working against you today.",
        "Well done! You've shown impressive dedication to the art of empty-handedness.",
        "You should really check your pockets. They must be full of all the things you can't pick up!",
        "Looks like you just earned the prestigious 'Master of the Empty Hand' award! Wear it with pride!"
    };
#define NO_ITEM_ERROR_SZ 10


char *WEAPON_NAME_BY_TYPE[] = {"Mace", "Dagger", "Magic Wand", "Normal Arrow", "Sword"};

char *CURSE_NAME_BY_TYPE[] = {"Health", "Speed", "Damage"};
char *CURSE_EXPLAIN_BY_TYPE[] = {"You regain health faster", "Your will be faster", "Your weapons will be stronger"};

char *FOOD_NAME_BY_TYPE[] = {"Normal", "Power", "Speed", "Normal"};
char *REAL_FOOD_NAME_BY_TYPE[] = {"Normal", "Power", "Speed", "Poisoned"};
int FOOD_COLOR_BY_TYPE[] = {5, 5, 5, 3};

#define NO_FOOD_ERROR_SZ 15
const char* NO_FOOD_ERROR[] = {
    "You're so hungry, you'd fight a raccoon for a half-eaten sandwich... and lose.",
    "You lick the crumbs off the floor, only to realize they're just dirt. Pathetic.",
    "You open the fridge, and the light goes out. Even it's given up on you.",
    "You try to eat your own shadow, but it's too fast for you. How embarrassing.",
    "You stare at an empty plate, pretending it's a feast. Delusion is your only seasoning.",
    "You beg the vending machine for mercy, but it just laughs in 'Out of Order.'",
    "You consider eating the wallpaper. It's floral, so it's basically a salad, right? Wrong.",
    "You try to chew on your own hair, but even your split ends reject you.",
    "You sniff the air, hoping to catch a whiff of someone else's dinner. You're basically a food stalker now.",
    "You try to convince yourself that hunger is just a mindset. Your stomach calls you a liar.",
    "You check the trash can for leftovers, but even the mold has standards.",
    "You try to eat your own words, but they're too bitter to swallow.",
    "You stare at a picture of food so long, your phone locks itself out of pity.",
    "You try to drink water to feel full, but it just reminds you how empty your life is.",
    "You consider eating your homework, but it's not even edible. Fail."
};

#define NO_CURSE_ERROR_SZ 10
const char* NO_CURSE_ERROR[] = {
        "A potion? Oh, sweet summer child. We don't have that. Maybe try crying into a bottle and see if your tears work instead?",
        "You want a potion? Sorry, we're all out. But hey, I hear overconfidence and poor planning are great substitutes!",
        "Oh, you need a potion? That's cute. Unfortunately, we're fresh out of 'Liquid Luck for the Hopeless.' Try again never!",
        "A potion? HA! The only thing we have in stock is disappointment. Looks like you're already an expert at that, though.",
        "You want a potion? Wow, bold of you to assume we'd have something useful for someone like you. Try a health bar next time!",
        "Sorry, we're out of potions. But don't worry—your sheer lack of skill is its own kind of protection. You'll be fine!",
        "A potion? Oh, honey. We don't have those here. Maybe try closing your eyes and pretending you're better at this game?",
        "You want a potion? How quaint. Unfortunately, we're all out of 'Juice for the Clueless.' Maybe try not getting hit next time?",
        "A potion? Sorry, we're fresh out. But hey, at least your failure is entertaining for the rest of us!",
        "You need a potion? That's adorable. Unfortunately, we're out of stock. Maybe try being less bad at the game instead?"
    };


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

void pickUpFood(Tile *t) {
    assert(t->F);
    P->food[P->num_food] = t->F;
    P->food[P->num_food]->pickup_time = get_game_timer();
    P->num_food++;

    char *s = (char *)malloc(100 * sizeof(char));
    sprintf(s, "Hmm... A %s Food Picked up!", FOOD_NAME_BY_TYPE[t->F->type]);
    setTopMsg(s);

    t->F = NULL;
    t->type = 0;
}

void pickUpCurse(Tile *t) {
    assert(t->C);
    P->curse[P->num_curse] = t->C;
    P->num_curse++;

    char *s = (char *)malloc(100 * sizeof(char));
    sprintf(s, "You picked up %s Curse!", CURSE_NAME_BY_TYPE[t->C->type]);
    setTopMsg(s);

    t->C = NULL;
    t->type = 0;
}

//TODO (MORE ITEMS)
void searchItem(Tile *t) {
    int type = t->type;
    if (type != 42) {
        setTopMsg(NO_ITEM_ERROR[rand() % NO_ITEM_ERROR_SZ]);
        return;
    }
    if (t->G) assert(false);
    if (t->F) {
        pickUpFood(t);
    }
    else if (t->C) {
        pickUpCurse(t);
    }
    else if (t->W) {
       pickUpWeapon(t);
    }
}

void showWeapon() {
    char *menu[MAX_WEAPON], *msg[MAX_WEAPON];
    int ind[MAX_WEAPON];
    int sz = 0;
    for (int i = 0; i < MAX_WEAPON; ++i) {
        if (P->weapon[i].td) {
            menu[sz] = WEAPON_NAME_BY_TYPE[P->weapon[i].type];
            ind[sz] = i;

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
    int x = createMenu(menu, msg, "Weapon", sz);
    if (x == -1) return;
    P->def_weapon = ind[x];
}

void consumeFood(Food *F) {
    char *s = (char *)malloc(100 * sizeof(char));
    sprintf(s, "You ate %s Food", REAL_FOOD_NAME_BY_TYPE[F->type]);
    renderMsgAndWait(s, FOOD_COLOR_BY_TYPE[F->type]);
    if (F->type != 3) {
        if (P->hunger < MAX_HUNGER)
            P->hunger = MAX_HUNGER;
        else
            P->health = P->max_health;
        if (F->type == 1)
        {
            P->damage_mult = 2;
            P->damage_mult_last_time = get_game_timer();
        }
        if (F->type == 2)
        {
            P->speed_mult = 2;
            P->speed_mult_last_time = get_game_timer();
        }
    }
    else {
        P->hunger = 0;
        P->health /= 2;
    }
}

void showFood() {
    char *menu[MAX_FOOD], *msg[MAX_FOOD];
    int ind[MAX_FOOD];
    int sz = 0;
    for (int i = 0; i < MAX_FOOD; ++i) {
        if (P->food[i] != NULL) {
            menu[sz] = FOOD_NAME_BY_TYPE[P->food[i]->type];
            msg[sz] = "Food...?";
            ind[sz] = i;
            ++sz;
        }
    }
    if (sz == 0) {
        renderMsgAndWait(NO_FOOD_ERROR[rand() % NO_FOOD_ERROR_SZ], 3);
        return;
    }
    int x = createMenu(menu, msg, "Food", sz);
    if (x == -1) return;
    x = ind[x];
    //You should eat x'th food
    consumeFood(P->food[x]);
    P->food[x] = NULL;
}

//Health, Speed, Damage
void consumeCurse(Curse *C) {
    switch (C->type) {
        case 0:
            P->health_recover_mult = 2;
            P->health_recover_mult_last_time = get_game_timer();
            break;
        case 1:
            P->speed_mult = 2;
            P->speed_mult_last_time = get_game_timer();
            break;
        case 2:
            P->damage_mult = 2;
            P->damage_mult_last_time = get_game_timer();
            break;
        default:
            assert(false);
    }
}

void showCurse() {
    char *menu[MAX_CURSE], *msg[MAX_CURSE];
    int ind[MAX_CURSE];
    int sz = 0;
    for (int i = 0; i < MAX_CURSE; ++i) {
        if (P->curse[i] != NULL) {
            menu[sz] = CURSE_NAME_BY_TYPE[P->curse[i]->type];
            msg[sz] = CURSE_EXPLAIN_BY_TYPE[P->curse[i]->type];
            ind[sz] = i;
            ++sz;
        }
    }
    if (sz == 0) {
        renderMsgAndWait(NO_CURSE_ERROR[rand() % NO_CURSE_ERROR_SZ], 3);
        return;
    }
    int x = createMenu(menu, msg, "Curse", sz);
    if (x == -1) return;
    x = ind[x];
    //You should eat x'th curse
    consumeCurse(P->curse[x]);
    P->curse[x] = NULL;
}

void showInventory() {
    char *menu[3] = {"Weapon", "Food", "Curse"};
    char *msg[3] = {"Show Weapons in your backpack", "Show Food in your backpack", "Show Curses in your backpack"};

    int x = createMenu(menu, msg, "Backpack", 3);
    switch (x) {
        case -1:
            return;
        case 0:
            showWeapon();
            return;
        case 1:
            showFood();
            return;
        case 2:
            showCurse();
            return;
        default:
            return;
    }
}

//Foods should be poisned after sometime and ...
void processFoodOverTime() {
    int changed = 0;
    for (int i = 0; i < P->num_food; ++i) if (P->food[i] != NULL && get_game_timer() - P->food[i]->pickup_time >= TIME_OF_DEGRADING_OF_FOOD) {
        int t = P->food[i]->type;
        if (t == 0) P->food[i]->type = 3; //to poison
        else if (t == 1 || t == 2) P->food[i]->type = 0;
        P->food[i]->pickup_time = get_game_timer();
        changed = 1;
    }
    if (changed) setTopMsg("Some of your food(s) corrupted");
}