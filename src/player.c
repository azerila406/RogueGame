#include "game.h"

void initPlayer(Player* P, Level* L, int x, int y, int max_health) {
  P->max_health = max_health;
  P->x = x;
  P->y = y;
  P->num_item = 0;
  P->exp = 0;
  P->gold = 0;
}

void movePlayer(Level* L, int x, int y) {
  if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH || (L->tile[x][y].type & 1))
    return;
  P->x = x;
  P->y = y;
}