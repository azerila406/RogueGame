#include "game.h"

// TODO add getting health
void processPlayer() {
  processFoodOverTime();

  if (P->hunger == MAX_HUNGER) {
    if (P->health < P->max_health) {
      P->health = max(P->max_health, P->health + P->health_recover_mult);
    }
  }

  if (P->hunger > 0) {
    if (get_game_timer() - P->last_time_hunger >= TIME_OF_HUNGER_DECREASE) {
      P->hunger--;
    }
  } else
    P->health--;

  if (P->damage_mult == 2 && (get_game_timer() - P->damage_mult_last_time) >=
                                 TIME_OF_DAMAGE_MULT_LASTING) {
    P->damage_mult = 1;
    setTopMsg("You lost your power :(((");
  }

  if (P->speed_mult == 2 && (get_game_timer() - P->speed_mult_last_time) >=
                                TIME_OF_SPEED_MULT_LASTING) {
    P->speed_mult = 1;
    setTopMsg("You lost your speed :(((");
  }

  if (P->health_recover_mult == 2 &&
      (get_game_timer() - P->health_recover_mult_last_time) >=
          TIME_OF_HEALTH_RECOVER_MULT_LASTING) {
    P->health_recover_mult = 1;
    setTopMsg("You lost your special healing power :((((");
  }
}

void initPlayer(Player* P, Level* L, int max_health) {
  P->max_health = P->health = max_health;
  P->num_curse = 0;
  P->num_food = 0;
  P->exp = 0;
  P->gold = 0;
  P->damage_mult = P->speed_mult = P->health_recover_mult = 1;
  P->hunger = MAX_HUNGER;
  P->last_time_hunger = get_game_timer();
  P->score = 0;

  for (int i = 0; i < MAX_WEAPON; ++i) {
    P->weapon[i].type = i;
    P->weapon[i].td = 0;
    P->weapon[i].dmg = WEAPON_DAMAGE_BY_TYPE[i];
    P->weapon[i].range = WEAPON_RANGE_BY_TYPE[i];
  }
  P->weapon[0].td = -1;  // INF
  P->def_weapon = 0;

  for (int i = 0; i < MAX_FOOD; ++i) P->food[i] = NULL;
  for (int i = 0; i < MAX_CURSE; ++i) P->curse[i] = NULL;

  P->x = -1;
  do {
    int r = rand() % L->num_room;
    // TODO probably some conditions on the room
    P->x = rnd(L->room[r].x + 1, L->room[r].x + L->room[r].height - 1);
    P->y = rnd(L->room[r].y + 1, L->room[r].y + L->room[r].width - 1);
  } while (P->x == -1 || L->tile[P->x][P->y].type != 0);
}

void discoverTile(Level* L, int x, int y) {
  int t = L->tile[x][y].type;
  if (t == 12 || t == 14) {  // HIDDEN DOOR
    L->tile[x][y].type = 6;  // Discovered
  }
  if (t == 22) {
    renderMsgAndWait("You stepped on a trap", 3);
    P->health -= 3;
  }
  discoverItem(&L->tile[x][y]);
}

void movePlayer(Level* L, int x, int y) {
  if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) return;
  int t = L->tile[x][y].type;
  if (t & 1) return;
  if (t == 18) {  // PASS DOOR
    int q = passDoorLogin(&L->tile[x][y].lock);
    if (q == 1) L->tile[x][y].type = 20;
    return;
  }
  if (t == 16) {  // PASS GENERATOR
    reinitLock(&L->tile[L->tile[x][y].lockx][L->tile[x][y].locky].lock);
    return;
  }
  P->x = x;
  P->y = y;
  discoverTile(L, x, y);
}

void moveStairs(Level* L, int where) {
  int x = P->x, y = P->y, t = L->tile[x][y].type;
  if (where == -1) {
    if (t == 10) {
      G->cur--;
    }
  } else if (where == +1) {
    if (t == 8) {
      G->cur++;
    }
  }
}

bool checkUnseen(Level* L, int x, int y) {
  int t = L->tile[x][y].type;
  if (t == 0 || t == 2 || t == 8 || t == 10) return 1;
  return 0;
}

void processUnseen(Level* L) {
  int r = whichRoomID(L, P->x, P->y);
  if (r != -1 && L->room[r].type != 3) L->room[r].visible = 1;
  BFS(L, P->x, P->y, checkUnseen);
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      int t = L->tile[i][j].type;
      if (dist[i][j] <= HALLWAY_SIGHT) {
        L->tile[i][j].visible = 1;
      }
    }
  }
}