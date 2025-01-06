#include "game.h"

Game *G;
Player *P;

void gameloop() {
  clear();

  initGame();

  while (1) {
    processUnseen(&(G->lvl[G->cur]));
    render(&(G->lvl[G->cur]));
    int ch = getch();
    if (ch == 'q') return;
    processInput(ch, &(G->lvl[G->cur]));
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