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
      return 2;
  } else if (t->F) {
    return 1;
  } else if (t->C) {
    return t->C->type + 1;
  } else if (t->W) {
    return 1;
  }
  assert(0);
}

int getColor(Tile *t) {
  switch (t->type) {
    case 0:
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
    case 4:
      return 4;
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

char enemyChar(Enemy *E) {
  return ENEMY_CHAR_BY_TYPE[E->type];
}

wchar_t *itemChar(Tile *t) {
  if (t->G) {
    return L"G";
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
      return L".";
    case 12:
    case 1:
      return L"|";
    case 2:
      return L"#";
    case 14:
    case 3:
      return L"-";
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
    case 4:
      return L"O";
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
  for (int i = 0; i < WIDTH + 30; ++i) mvprintw(0, i, " "); //TODO
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

void renderHUD(Level *l) {
  clearMsg();
  renderMsg(top_msg, 1);
  attron(COLOR_PAIR(1));
  for (int i = 0; i < WIDTH + 30; ++i)
    mvprintw(HEIGHT + 2, i, " "); // TODO

  mvprintw(
      HEIGHT + 2, 0,
      " Level: %d/%d   Health: %d/%d   Gold: %d   Exp: %d   Hunger: %d/%d   Weapon: %s/%d   dmg/speed/healthX: %d/%d/%d",
      G->cur + 1, G->num_level, P->health, P->max_health, P->gold, P->exp, P->hunger, MAX_HUNGER, WEAPON_NAME_BY_TYPE[P->def_weapon], P->weapon[P->def_weapon].td, P->damage_mult, P->speed_mult, P->health_recover_mult);
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
        mvprintw(i + 1, (j + 1), "%ls", tileChar(t));
        attroff(COLOR_PAIR(getColor(t)));
      } else {
        attron(COLOR_PAIR(1));
        mvprintw(i + 1, (j + 1), " ");
        attroff(COLOR_PAIR(1));
      }
    }
  }
  //print enemies:

  attron(COLOR_PAIR(6));
  for (int i = 0; i < l->num_enemy; ++i) if (l->enemy[i].health > 0) {
    int x = l->enemy[i].x;
    int y = l->enemy[i].y;
    Tile *t = &(l->tile[x][y]);
    int r = whichRoomID(l, x, y);
    bool vis =
        toggle_map_status | t->visible | (r != -1 && l->room[r].visible);
    if (vis) mvprintw(l->enemy[i].x + 1, l->enemy[i].y + 1, "%c", enemyChar(&l->enemy[i]));
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