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
}

void initRoomHallway(Level* L) {
  for (int i = 0; i < L->num_room; ++i) {
    initRoom(L, &(L->room[i]));
  }
  initHallway(L);
}

void initLevel(Level* L) {
  do {
    initRooms(L);
  } while (roomsCollide(L->room, L->num_room));
  initRoomHallway(L);
}

void initLevelWithARoom(Level *L, Room *R) {

}