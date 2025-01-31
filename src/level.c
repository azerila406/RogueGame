#include "game.h"

bool collide(int l0, int r0, int l1, int r1) {
  int L = max(l0, l1);
  int R = min(r0, r1);
  if (L <= R) return 1;
  return 0;
}

bool roomsCollide(Room r[], int n) {
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; ++j) {
      if (collide(r[i].x, r[i].x + r[i].height, r[j].x, r[j].x + r[j].height) &&
          collide(r[i].y, r[i].y + r[i].width, r[j].y, r[j].y + r[j].width))
        return 1;
    }
  }
  return 0;
}

void initRooms(Level* L) {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      L->tile[i][j].type = 5;
      L->tile[i][j].visible = 0;
    }
  }
  L->num_room = rnd(MIN_ROOMS_PER_LEVEL, MAX_ROOMS_PER_LEVEL);

  for (int i = 0; i < L->num_room; ++i) {
    L->room[i].width = rnd(MIN_INTERIOR_OF_ROOM, MAX_INTERIOR_OF_ROOM);
    L->room[i].height = rnd(MIN_INTERIOR_OF_ROOM, MAX_INTERIOR_OF_ROOM);
    L->room[i].x = rnd(2, HEIGHT - L->room[i].height - 2);
    L->room[i].y = rnd(2, WIDTH - L->room[i].width - 2);
    L->room[i].type = 0;
    L->room[i].visible = 0;
  }
}

void makeDoorHidden(Level* L, int r) {
  int x0 = getX0(&(L->room[r])), x1 = getX1(&(L->room[r])),
      y0 = getY0(&(L->room[r])), y1 = getY1(&(L->room[r]));

  for (int i = x0; i <= x1; ++i) {
    for (int j = y0; j <= y1; ++j) {
      if (L->tile[i][j].type == 6)
        L->tile[i][j].type = (i == x0 || i == x1 ? 14 : 12);
    }
  }
}

void makePassDoor(Level* L, int r) {
  int x0 = getX0(&(L->room[r])), x1 = getX1(&(L->room[r])),
      y0 = getY0(&(L->room[r])), y1 = getY1(&(L->room[r]));

  Vector vecx, vecy;
  vec_init(&vecx);
  vec_init(&vecy);
  for (int i = x0; i <= x1; ++i)
    for (int j = y0; j <= y1; ++j) {
      if (L->tile[i][j].type == 6) {
        vec_push(&vecx, i);
        vec_push(&vecy, j);
      }
    }
  if (vecx.sz == 0) {  // we have only secret doors so ignore them
    return;
  }
  int R = rand() % vecx.sz;
  int i = vecx.a[R];
  int j = vecy.a[R];

  int X[2] = {x0 + 1, x1 - 1};
  int Y[2] = {y0 + 1, y1 - 1};
  int sx = X[rand() & 1], sy = Y[rand() & 1];
  initLock(&L->tile[i][j].lock, 0);
  L->tile[sx][sy].lockx = i;
  L->tile[sx][sy].locky = j;

  L->tile[sx][sy].type = 16;
  L->tile[i][j].type = 18;

  vec_free(&vecx);
  vec_free(&vecy);
}

void initHallway(Level* L) {
  DSU dsu;
  int E[10][3], e;
  bool ok;
  do {
    ok = 1;
    initDSU(&dsu, L->num_room);
    // randomly connect two rooms
    e = rnd(L->num_room - 1, L->num_room + 1);
    for (int i = 0; i < e; i++) {
      int x = rnd(0, L->num_room - 1), y;
      do {
        y = rnd(0, L->num_room - 1);
      } while (x == y);
      assert(y != x);
      int tx, ty;
      if (rand() & 1) {
        tx = rnd(L->room[x].x + 1, L->room[x].x + L->room[x].height - 1);
        ty = rand() & 1 ? getY0(&(L->room[x])) : getY1(&(L->room[x]));
      } else {
        ty = rnd(L->room[x].y + 1, L->room[x].y + L->room[x].width - 1);
        tx = rand() & 1 ? getX0(&(L->room[x])) : getX1(&(L->room[x]));
      }
      ok &= bfsDoor(L, tx, ty, y, 0);  //???
      merge(&dsu, x, y);
      E[i][0] = tx;
      E[i][1] = ty;
      E[i][2] = y;
    }
  } while (!ok || dsu.connected_component != 1);  // graph of rooms are not
                                                  // connected
  for (int i = 0; i < e; i++) {
    bfsDoor(L, E[i][0], E[i][1], E[i][2], 1);
  }
  makeDoorHidden(L, rnd(0, L->num_room - 1));
}

void initRoomHallway(Level* L) {
  for (int i = 0; i < L->num_room; ++i) {
    initRoom(L, &(L->room[i]));
  }
  initHallway(L);
}


void initFood(Tile *t) {
  t->type = 42;
  t->F = (Food*) malloc(sizeof (Food));
  t->F->type = rand() % 4;
}

int WEAPON_DAMAGE_BY_TYPE[] = {5, 12, 15, 5, 10};
int WEAPON_RANGE_BY_TYPE[] = {0, 5, 10, 5, 0};
int WEAPON_TD_BY_TYPE[] = {-1, 10, 8, 20, -1};

void initWeapon(Tile *t) {
  t->type = 42;
  t->W = (Weapon*) malloc(sizeof (Weapon));
  t->W->type = rnd(1, MAX_WEAPON - 1);
  t->W->td = WEAPON_TD_BY_TYPE[t->W->type];
  t->W->range = WEAPON_RANGE_BY_TYPE[t->W->type];
  t->W->dmg = WEAPON_DAMAGE_BY_TYPE[t->W->type];
}

void initCurse(Tile *t) {
  t->type = 42;
  t->C = (Curse*) malloc(sizeof (Curse));
  t->C->type = rand() % 3;
}

void initGold(Tile *t) {
  t->type = 42;
  t->G = (Gold*) malloc(sizeof (Gold));
  t->G->type = rand() % 8 == 0; //(1 / 8) being Black
  t->G->gold = (t->G->type == 1 ? rnd(50, 100) : rnd(1, 10)); //Gold Values
}

//Food, Weapon, Curse, Gold
//TODO ROOMS TYPE
//prob is partial sum of x/50
const int ROOMS_TYPE_PROB[4][5] = {
  {1, 3, 5, 7}, 
  {1, 5, 6, 15}, 
  {7, 7, 14, 14},
  {0, 0, 0, 0}
};

//out of 20
const int ROOMS_ENEMY_PROB_BY_TYPE[4] = {2, 5, 0, 3};

void initItemsRoom(Level *L, Room *R) {
  int x0 = getX0(R), x1 = getX1(R),
    y0 = getY0(R), y1 = getY1(R);
  int tp = R->type;
  for (int i = x0; i <= x1; ++i) {
    for (int j = y0; j <= y1; ++j) if (L->tile[i][j].type == 0) {
      int r = rand() % 50 + 1;
      void (*initFunctions[])(Tile *) = {
          initFood,
          initWeapon,
          initCurse,
          initGold};
      for (int k = 0; k < 4; k++) {
        if (r <= ROOMS_TYPE_PROB[tp][k]) {
          initFunctions[k](&L->tile[i][j]);
          break;
        }
      }
    }
  }

  for (int i = x0; i <= x1; ++i) {
    for (int j = y0; j <= y1; ++j) if (L->tile[i][j].type == 0) {
      int r = rand() % 20;
      if (r < ROOMS_ENEMY_PROB_BY_TYPE[tp]) {
        L->enemy[L->num_enemy].x = i;
        L->enemy[L->num_enemy].y = j;
        initEnemy(&L->enemy[L->num_enemy]);
        L->num_enemy++;
      }
    }
  }
}

void initItemsLevel(Level *L) { //ROOM TYPE MATTERS HERE: TODO
  for (int i = 0; i < L->num_room; ++i) {
    initItemsRoom(L, &L->room[i]);
  }
}

void initEnemies(Level *L) {
  L->num_enemy = rand() % MAX_ENEMEY;
  for (int i = 0; i < L->num_enemy; ++i) {
    int x, y;
    do {
      x = rand() % HEIGHT;
      y = rand() % WIDTH;
    } while (L->tile[x][y].type);
    L->enemy[i].x = x;
    L->enemy[i].y = y;
    initEnemy(&L->enemy[i]);
  }
}

void initSotoon(Level *lvl) {
  int x, y;
  do {
    x = rand() % HEIGHT;
    y = rand() % WIDTH;
  } while (lvl->tile[x][y].type);
  lvl->tile[x][y].type = 21;
}

//out of /20
const int TRAP_PROB_BY_TYPE[] = {1, 4, 1, 1};

void initTrap(Level *lvl) {
  for (int x = 0; x < HEIGHT; ++x) {
    for (int y = 0; y < WIDTH; ++y) {
      Tile *t = &lvl->tile[x][y];
      if (t->type == 0) {
        int r = rand() % 20;
        if (r < TRAP_PROB_BY_TYPE[t->room_type]) {
          t->type = 22;
        }
      }
    }
  }
}

void expandTypeToAllTiles(Level *L, Room *R) {
  for (int i = getX0(R); i <= getX1(R); ++i)
    for (int j = getY0(R); j <= getY1(R); ++j)
      L->tile[i][j].room_type = R->type;
}

void initRoomsType(Level *L) {
  for (int i = 0; i < L->num_room; ++i) {
    if (rand() % 5) L->room[i].type = 0; //Normal
    else {
      int found_stairs = isThereStairs(L, &L->room[i]);
      if (found_stairs) L->room[i].type = 2; //Enchant
      else L->room[i].type = 3; //Nightmare
    }
    expandTypeToAllTiles(L, &L->room[i]);
  }
}


void initLevel(Level* L) {
  do {
    initRooms(L);
  } while (roomsCollide(L->room, L->num_room));
  initRoomHallway(L);
  //initEnemies(L);
  initSotoon(L);
  initTrap(L);
  initRoomsType(L);
}

void initLevelWithARoom(Level* L, Room* R) {
  do {
    initRooms(L);
    memcpy(&(L->room[0]), R, sizeof(Room));
    // TODO maybe change type of ROOM?
  } while (roomsCollide(L->room, L->num_room));
  initRoomHallway(L);
  //initEnemies(L);
  initSotoon(L);
  initTrap(L);
  initRoomsType(L);
}