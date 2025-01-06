#include "game.h"

#define DIFF_LEVEL 3
const int DIFF_LEVELS[4] = {100, 50, 25, 10};

bool toggle_map_status;

void initGame() {
  toggle_map_status = 0;
  P = (Player *)malloc(sizeof(Player));
  G = (Game *)malloc(sizeof(Game));
  G->cur = 0;
  initLevel(&(G->lvl[0]));
  initPlayer(P, &(G->lvl[0]), DIFF_LEVELS[DIFF_LEVEL]);
  G->num_level = rnd(MIN_LEVELS, MAX_LEVELS);
  int lst_r = whichRoomID(&(G->lvl[0]), P->x, P->y);
  for (int i = 0; i + 1 < G->num_level; ++i) {
    int r, x, y;
    do {
      r = rand() % G->lvl[i].num_room;
      x = rnd(getX0(&(G->lvl[i].room[r])) + 1, getX1(&(G->lvl[i].room[r])) - 1);
      y = rnd(getY0(&(G->lvl[i].room[r])) + 1, getY1(&(G->lvl[i].room[r])) - 1);
    } while (G->lvl[i].tile[x][y].type || lst_r == r);
    G->lvl[i].tile[x][y].type = 8;
    do {
      initLevelWithARoom(&(G->lvl[i + 1]), &(G->lvl[i].room[r]));
    } while (G->lvl[i + 1].tile[x][y].type);
    G->lvl[i + 1].tile[x][y].type = 10;
    lst_r = whichRoomID(&(G->lvl[i + 1]), x, y);
  }
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
    case 'u':
    case 'U':
      movePlayer(L, P->x - 1, P->y + 1);
      return;
    case 'y':
    case 'Y':
      movePlayer(L, P->x - 1, P->y - 1);
      return;
    case 'n':
    case 'N':
      movePlayer(L, P->x + 1, P->y + 1);
      return;
    case 'B':
    case 'b':
      movePlayer(L, P->x + 1, P->y - 1);
      return;
    case KEY_UP:
      moveStairs(L, +1);
      return;
    case KEY_DOWN:
      moveStairs(L, -1);
      return;
    case 'm':
    case 'M':
      toggle_map_status ^= 1;
      return;
    default:
      setTopMsg("Illegal Command");
      return;
  }
}