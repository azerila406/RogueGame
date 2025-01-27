#include "game.h"

Game *G;
Player *P;

int game_movement_timer = 0;

void gameloop() {
  clear();

  initGame();

  while (1) {
    checkTimerMsg();
    processPlayer();
    processUnseen(&(G->lvl[G->cur]));
    render(&(G->lvl[G->cur]));
    int ch = getch();
    if (ch == 'q') return;
    processInput(ch, &(G->lvl[G->cur]));
    game_movement_timer++;
  }
}

int get_game_timer() {
  return game_movement_timer;
}

signed main() {
  setlocale(LC_ALL, "");
  srand(time(0));
  initScreen();
  gameMenu();
  // userMenu();
  //  gameloop();
  endwin();
  return 0;
}