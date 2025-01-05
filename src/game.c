#include "game.h"

#define DIFF_LEVEL 3
const int DIFF_LEVELS[4] = {100, 50, 25, 10};

void initGame() {
  P = (Player *)malloc(sizeof(Player));
  G = (Game *)malloc(sizeof(Game));
  G->cur = 0;
  initLevel(&(G->lvl[0]));
  initPlayer(P, &(G->lvl[G->cur]), DIFF_LEVELS[DIFF_LEVEL]);
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