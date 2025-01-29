#include "game.h"

int getX0(Room *r) { return r->x; }
int getX1(Room *r) { return r->x + r->height; }
int getY0(Room *r) { return r->y; }
int getY1(Room *r) { return r->y + r->width; }

void initRoom(Level *L, Room *R) {
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

int isThereStairs(Level *L, Room *R) {
  for (int i = getX0(R); i <= getX1(R); ++i) {
    for (int j = getY0(R); j <= getY1(R); ++j) {
      if (L->tile[i][j].type == 8) return 1;
    }
  }
  return 0;
}