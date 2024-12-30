#include "game.h"

int getColor(Tile *t) {
  switch (t->type) {
    case 0:
      return 2;
    case 5:
      return 1;
    case 1:
    case 3:
    case 2:
    case 7:
      return 4;
    default:
      exit(5);
  }
}

char tileChar(Tile *t) {
  switch (t->type) {
    case 0:
      return '.';
    case 1:
      return '|';
    case 2:
      return '#';
    case 3:
      return '-';
    case 5:
      return ' ';
    case 7:
      return '+';
    default:
      exit(5);
  }
}

void initScreen() {
  initscr();
  if (!has_colors()) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  start_color();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);

  refresh();
}

void renderMsg(const char *s) {
  attron(COLOR_PAIR(2));
  mvprintw(0, 1, s);
  attroff(COLOR_PAIR(2));
}

void renderHUD(Level *l) {
  attron(COLOR_PAIR(1));
  mvprintw(HEIGHT + 2, 0,
           " Level: %d    Health: %d / %d    Gold: %d    Exp: %d    Items: %d",
           l->lvl_num, P->health, P->max_health, P->gold, P->exp, P->num_item);
  attroff(COLOR_PAIR(1));
}

/* Render each Frame
 +1 Everything */
void render(Level *l) {
  renderHUD(l);
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      Tile *t = &(l->tile[i][j]);
      // TODO
      // bool vis = t->visible | t->room->visible;
      if (1) {
        attron(COLOR_PAIR(getColor(t)));
        mvprintw(i + 1, j + 1, to_string(tileChar(t)));
        attroff(COLOR_PAIR(getColor(t)));
      }
    }
  }
  attron(COLOR_PAIR(1));
  mvprintw(P->x + 1, P->y + 1, "@");
  move(P->x + 1, P->y + 1);
  attroff(COLOR_PAIR(1));
}