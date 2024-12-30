#include "game.h"

void initRoom(Level* L, Room* R) {
  int x0 = R->x, x1 = R->x + R->height, y0 = R->y, y1 = R->y + R->width;
  for (int i = x0; i <= x1; ++i) {
    for (int j = y0; j <= y1; ++j) {
      if (i == x0 || i == x1) {
        L->tile[i][j].type = 3;
      } else if (j == y0 || j == y1) {
        L->tile[i][j].type = 1;
      } else {
        L->tile[i][j].type = 0;
      }
    }
  }
}

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

void initLevel(Level* L) {
  L->num_room = rnd(MIN_ROOMS_PER_LEVEL, MAX_ROOMS_PER_LEVEL);

  do {
    for (int i = 0; i < L->num_room; ++i) {
      L->room[i].width = rnd(MIN_INTERIOR_OF_ROOM, MAX_INTERIOR_OF_ROOM);
      L->room[i].height = rnd(MIN_INTERIOR_OF_ROOM, MAX_INTERIOR_OF_ROOM);
      L->room[i].x = rnd(2, HEIGHT - L->room[i].height - 2);
      L->room[i].y = rnd(2, WIDTH - L->room[i].width - 2);
      L->room[i].type = 0;
      L->room[i].visible = 0;
    }
  } while (roomsCollide(L->room, L->num_room));
  printf("YA\n");

  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      L->tile[i][j].type = 5;
    }
  }

  for (int i = 0; i < L->num_room; ++i) {
    initRoom(L, &(L->room[i]));
  }
}