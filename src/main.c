#include "game.h"

#define DIFF_LEVEL 3
const int DIFF_LEVELS[4] = {100, 50, 25, 10};

Game *G;
Player *P;

void gameloop() {
  clear();
  P = (Player *)malloc(sizeof(Player));
  G = (Game *)malloc(sizeof(Game));
  G->cur = 0;
  initLevel(&(G->lvl[G->cur]));
  initPlayer(P, &(G->lvl[G->cur]), DIFF_LEVELS[DIFF_LEVEL]);

  render(&(G->lvl[G->cur]));
  int ch = -1;
  while ((ch = getch()) != 'q') {
    processInput(ch, &(G->lvl[G->cur]));
    render(&(G->lvl[G->cur]));
  }
}

signed main() {
  initScreen();
  userMenu();
  // gameloop();
  endwin();
  return 0;
}