#include "game.h"

Game *G;
Player *P;

void gameloop() {
  clear();

  initGame();

  render(&(G->lvl[G->cur]));
  int ch = -1;
  while ((ch = getch()) != 'q') {
    processInput(ch, &(G->lvl[G->cur]));
    render(&(G->lvl[G->cur]));
  }
}

signed main() {
  initScreen();
  gameMenu();
  // userMenu();
  //  gameloop();
  endwin();
  return 0;
}