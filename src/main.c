#include "game.h"

#define DIFF_LEVEL 3
const int DIFF_LEVELS[4] = {100, 50, 25, 10};

Game *G;
Player *P;

void gameloop() {
  clear();
  P = (Player *)malloc(sizeof(Player));
  G = (Game *)malloc(sizeof(Game));
  initLevel(&(G->lvl[0]));
  initPlayer(P, &(G->lvl[0]), DIFF_LEVELS[DIFF_LEVEL]);

  render(&(G->lvl[0]));
  int ch = -1;
  while ((ch = getch()) != 'q') {
    processInput(ch, &(G->lvl[0]));
    render(&(G->lvl[0]));
  }
}

signed main() {
  initScreen();
  gameMenu();
  // gameloop();
  endwin();
  return 0;
}