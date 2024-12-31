#include "game.h"

void initPlayer(Player* P, Level* L, int max_health) {
  P->max_health = P->health = max_health;
  P->num_item = 0;
  P->exp = 0;
  P->gold = 0;

  P->x = -1;
  do {
    int r = rand() % L->num_room;
    // TODO probably some conditions on the room
    P->x = rnd(L->room[r].x + 1, L->room[r].x + L->room[r].height - 1);
    P->y = rnd(L->room[r].y + 1, L->room[r].y + L->room[r].width - 1);
  } while (P->x == -1);
}

void movePlayer(Level* L, int x, int y) {
  if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) return;
  int t = L->tile[x][y].type;
  if (t & 1) return;
  P->x = x;
  P->y = y;
}