#include "game.h"

void initGame(Game *g) {
  initLevel(&(g->lvl[0]));
  g->lvl[0].lvl_num = 0;
  // TODO DEBUGGING
  /*
  for (int i = 0; i < MAX_LEVELS; i++) {

  }
  */
}

void processInput(int x, Level *L) {
  switch (x) {
    case 'w':
    case 'W':
      movePlayer(L, P->x - 1, P->y);
      return;
    case 'a':
    case 'A':
      movePlayer(L, P->x, P->y - 1);
      return;
    case 's':
    case 'S':
      movePlayer(L, P->x + 1, P->y);
      return;
    case 'D':
    case 'd':
      movePlayer(L, P->x, P->y + 1);
      return;
    case KEY_UP:
      moveStairs(L, +1);
      return;
    case KEY_DOWN:
      moveStairs(L, -1);
      return;
  }
}