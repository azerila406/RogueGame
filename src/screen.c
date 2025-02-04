#include "game.h"

/*
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_RED, COLOR_YELLOW);
  */

char *top_msg = "";
time_t timer_clock;
int MAIN_COLOR;

int itemColor(Tile *t) {
  if (t->G) {
    if (t->G->type == 0)
      return 1;
    else
      return 5;
  } else if (t->F) {
    return 1;
  } else if (t->C) {
    return t->C->type + 1;
  } else if (t->W) {
    return 1;
  }
  assert(0);
}

const int WALL_COLOR_BY_TYPE[] = {4, 1, 2, 3};

int wallColor(Tile *t) {
  int r = t->room_type;
  return WALL_COLOR_BY_TYPE[r];
}

int getColor(Tile *t) {
  switch (t->type) {
    case 0:
    case 22:
      return 2;
    case 16:
    case 5:
      return 1;
    case 1:
    case 3:
    case 2:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
    case 21:
    case 23:
    case 25:
    case 27:
    case 29:
      return wallColor(t);
    case 18:
      return 3;
    case 20:
      return 5;
    case 42:
      return itemColor(t);
    default:
      exit(17);
  }
}

const char ENEMY_CHAR_BY_TYPE[] = {'D', 'F', 'G', 'S', 'U'};

char enemyChar(Enemy *E) { return ENEMY_CHAR_BY_TYPE[E->type]; }

wchar_t *itemChar(Tile *t) {
  if (t->G) {
    return L"$";
  } else if (t->F) {
    return L"F";
  } else if (t->C) {
    return L"C️";
  } else if (t->W) {
    return L"W";
  }
  assert(0);
}

wchar_t *tileChar(Tile *t) {
  switch (t->type) {
    case 0:
    case 22:
      return L".";
    case 12:
    case 1:
      return L"│";
    case 2:
      return L"█";
    case 14:
    case 3:
      return L"─";
    case 5:
      return L" ";
    case 6:
      return L"+";
    case 8:
      return L"^";
    case 10:
      return L"v";
    case 16:
      return L"&";
    case 18:
    case 20:
      return L"@";
    case 42:
      return itemChar(t);
    case 21:
      return L"O";
    case 23:
      return L"┌";
    case 25:
      return L"┐";
    case 27:
      return L"┘";
    case 29:
      return L"└";
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
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_RED, COLOR_YELLOW);

  refresh();
}

void clearMsg() {
  attron(COLOR_PAIR(1));
  for (int i = 0; i < WIDTH + 30; ++i) mvprintw(0, i, " ");  // TODO
  attroff(COLOR_PAIR(1));
}

void renderMsg(const char *s, int color) {
  clearMsg();
  attron(COLOR_PAIR(color));
  mvprintw(0, 1, s);
  attroff(COLOR_PAIR(color));
}

void renderMsgAndWait(const char *s, int color) {
  clear();
  renderMsg(s, color);
  move(0, 1);
  getch();
}

const wchar_t *generateBar(int current, int max, int width) {
  static wchar_t bar[21];
  int filled = (current * width) / max;
  int empty = width - filled;

  wmemset(bar, L'█', filled);
  wmemset(bar + filled, L'░', empty);
  bar[width] = L'\0';
  return bar;
}

void renderHUD(Level *l) {
  clearMsg();
  renderMsg(top_msg, 1);

  attron(COLOR_PAIR(2));
  for (int i = 0; i < WIDTH + 30; ++i) {
    mvprintw(HEIGHT + 2, i, " ");
    mvprintw(HEIGHT + 3, i, " ");
  }
  attroff(COLOR_PAIR(2));

  int hud_y = HEIGHT + 2;

  attron(COLOR_PAIR(1));
  mvprintw(hud_y, 0, " Level: %d/%d ", G->cur + 1, G->num_level);
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(5));
  mvprintw(hud_y, 15, "Health: [%ls] %d/%d",
           generateBar(P->health, P->max_health, 10), P->health, P->max_health);
  attroff(COLOR_PAIR(5));

  attron(COLOR_PAIR(1));
  mvprintw(hud_y, 44, "Gold: %d", P->gold);
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(4));
  mvprintw(hud_y, 54, "Exp: %d ", P->exp);
  attroff(COLOR_PAIR(4));

  attron(COLOR_PAIR(3));
  mvprintw(hud_y, 65, "Hunger: [%ls]", generateBar(P->hunger, MAX_HUNGER, 10));
  attroff(COLOR_PAIR(3));

  attron(COLOR_PAIR(6));
  mvprintw(hud_y + 1, 1, "Weapon: %s", WEAPON_NAME_BY_TYPE[P->def_weapon]);

  if (P->speed_mult > 1) {
    attron(COLOR_PAIR(4));
    mvprintw(hud_y + 1, 30, "S");
    attroff(COLOR_PAIR(4));
  }

  if (P->damage_mult > 1) {
    attron(COLOR_PAIR(3));
    mvprintw(hud_y + 1, 32, "D");
    attroff(COLOR_PAIR(3));
  }

  if (P->health_recover_mult > 1) {
    attron(COLOR_PAIR(5));
    mvprintw(hud_y + 1, 34, "H");
    attroff(COLOR_PAIR(5));
  }
  attroff(COLOR_PAIR(6));
}

/* Render each Frame
 +1 Everything */
void render(Level *l) {
  renderHUD(l);
  refresh();
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      Tile *t = &(l->tile[i][j]);
      // TODO
      int r = whichRoomID(l, i, j);
      bool vis =
          toggle_map_status | t->visible | (r != -1 && l->room[r].visible);
      if (vis) {
        attron(COLOR_PAIR(getColor(t)));
        mvprintw(i + 1, (j + 1), "%ls", tileChar(t));
        attroff(COLOR_PAIR(getColor(t)));
      } else {
        attron(COLOR_PAIR(1));
        mvprintw(i + 1, (j + 1), " ");
        attroff(COLOR_PAIR(1));
      }
    }
  }

  // print enemies:
  attron(COLOR_PAIR(6));
  for (int i = 0; i < l->num_enemy; ++i)
    if (l->enemy[i].health > 0) {
      int x = l->enemy[i].x;
      int y = l->enemy[i].y;
      Tile *t = &(l->tile[x][y]);
      int r = whichRoomID(l, x, y);
      bool vis =
          toggle_map_status | t->visible | (r != -1 && l->room[r].visible);
      if (vis)
        mvprintw(l->enemy[i].x + 1, l->enemy[i].y + 1, "%c",
                 enemyChar(&l->enemy[i]));
    }
  attroff(COLOR_PAIR(6));

  attron(COLOR_PAIR(MAIN_COLOR));
  mvprintw(P->x + 1, (P->y + 1), "@");
  move(P->x + 1, (P->y + 1));
  attroff(COLOR_PAIR(MAIN_COLOR));
}

void setTopMsg(char *s) {
  top_msg = s;
  timer_clock = time(NULL);
}

void checkTimerMsg() {
  if (MSG_RESET_TIME <= difftime(time(NULL), timer_clock)) setTopMsg("");
}