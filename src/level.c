#include "game.h"

void initRoom() {}

bool roomsCollide(Room r[], int n) {
  for (int i = 0; i < n; i++) {
    int x0 = r[i].x, y0 = r[i].y;
    for (int j = i + 1; j < n; ++j) {
      int x1 = r[j].x, y1 = r[j].y;
      int mxx = max(x1, x0), mnx = min(x1, x0);
      int mxy = max(y1, y0), mny = min(y1, y0);
      if (mnx <= mxx && mny <= mxy) return 1;
    }
  }
  return 0;
}

void initLevel(Level *L) {
  L->num_room = rnd(6, 7);

  do {
    for (int i = 0; i < L->num_room; ++i) {
      L->room[i].width = rnd(6, 15);
      L->room[i].height = rnd(6, 15);
      L->room[i].x = rnd(2, HEIGHT - L->room[i].height - 2);
      L->room[i].y = rnd(2, WIDTH - L->room[i].width - 2);
      L->room[i].type = 0;
      L->room[i].visible = 0;
    }
  } while (roomsCollide(L->room, L->num_room));

  /*
    for (int i = 0; i < HEIGHT; ++i) {
      for (int j = 0; j < WIDTH; ++j) {
        Tile *t = &(L->tile[i][j]);
        t->num_item = 0;
        // t->visible = 0;
        // TODO DEBUGGING
        t->visible = 1;
        t->type = 0;
      }
    }
    */
}