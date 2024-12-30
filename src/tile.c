#include "game.h"

int whichRoomID(Level* L, int x, int y) {
  for (int i = 0; i < L->num_room; ++i) {
    int x0 = L->room[i].x, x1 = L->room[i].x + L->room[i].height;
    int y0 = L->room[i].y, y1 = L->room[i].y + L->room[i].width;
    if (x0 <= x && x <= x1 && y0 <= y && y <= y1) return i;
  }
  return -1;
}