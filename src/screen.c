#include "game.h"

void initScreen() {
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
}

void renderHUD(Level *l) {
  mvprintw(HEIGHT, 0,
           " Level: %d    Health: %d / %d    Gold: %d    Exp: %d    Items: %d",
           l->lvl_num, P->health, P->max_health, P->gold, P->exp, P->num_item);
}

/* Render each Frame */
void render(Level *l) {
  renderHUD(l);
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      Tile *t = &(l->tile[i][j]);
      // TODO
      // bool vis = t->visible | t->room->visible;
      if (1) {
        mvprintw(i, j, to_string(tileChar(t)));
      }
    }
  }
  mvprintw(P->x, P->y, "@");
  move(P->x, P->y);
}