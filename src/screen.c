#include "game.h"

char *top_msg = "";

int getColor(Tile *t) {
  switch (t->type) {
    case 0:
      return 2;
    case 5:
      return 1;
    case 1:
    case 3:
    case 2:
    case 6:
    case 8:
    case 10:
      return 4;
    default:
      exit(17);
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
    case 6:
      return '+';
    case 8:
      return '^';
    case 10:
      return 'v';
    default:
      exit(18);
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

void clearMsg() {
  attron(COLOR_PAIR(1));
  for (int i = 0; i < WIDTH; ++i) mvprintw(0, i, " ");
  attroff(COLOR_PAIR(1));
}

void renderMsg(const char *s) {
  clearMsg();
  attron(COLOR_PAIR(1));
  mvprintw(0, 1, s);
  attroff(COLOR_PAIR(1));
}

void renderMsgAndWait(const char *s) {
  clear();
  renderMsg(s);
  move(0, 1);
  getch();
}

void renderHUD(Level *l) {
  clearMsg();
  renderMsg(top_msg);
  attron(COLOR_PAIR(1));
  mvprintw(
      HEIGHT + 2, 0,
      " Level: %d / %d    Health: %d / %d    Gold: %d    Exp: %d    Items: %d",
      G->cur + 1, G->num_level, P->health, P->max_health, P->gold, P->exp,
      P->num_item);
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
      int r = whichRoomID(l, i, j);
      bool vis =
          toggle_map_status | t->visible | (r != -1 && l->room[r].visible);
      if (vis) {
        attron(COLOR_PAIR(getColor(t)));
        mvprintw(i + 1, j + 1, to_string(tileChar(t)));
        attroff(COLOR_PAIR(getColor(t)));
      } else {
        attron(COLOR_PAIR(1));
        mvprintw(i + 1, j + 1, " ");
        attroff(COLOR_PAIR(1));
      }
    }
  }
  attron(COLOR_PAIR(1));
  mvprintw(P->x + 1, P->y + 1, "@");
  move(P->x + 1, P->y + 1);
  attroff(COLOR_PAIR(1));

  top_msg = "";
}

void setTopMsg(char *s) { top_msg = s; }