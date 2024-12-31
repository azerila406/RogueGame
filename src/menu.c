#include "game.h"

void renderMenu(char *s[], char *msg[], int n, int x) {
  clear();
  attron(COLOR_PAIR(1));
  mvprintw(0, 0, msg[x]);
  attroff(COLOR_PAIR(1));

  for (int i = 0; i < n; i++) {
    if (i == x) attron(A_REVERSE | A_BOLD);
    mvprintw(i + 2, 2, s[i]);
    if (i == x) attroff(A_BOLD | A_REVERSE);
  }
  move(x + 2, 2);
}

int createMenu(char *s[], char *msg[], int n) {
  clear();
  int i = 0, ch;
  do {
    renderMenu(s, msg, n, i);
    ch = getch();
    switch (ch) {
      case KEY_UP:
      case 'w':
      case 'W':
        i = (n + i - 1) % n;
        continue;
      case KEY_DOWN:
      case 's':
      case 'S':
        i = (i + 1) % n;
        continue;
      case '\n':
      case '\r':
        return i;
    }
  } while (ch != 'q' && ch != 'Q');
  return -1;
}

void gameMenu() {
  char *s[2] = {"New Game", "Continue Game"};
  char *msg[2] = {"Starts a New Game", "Continues previous game if exists"};
  while (1) {
    int x = createMenu(s, msg, 2);
    // TODO for now
    if (x == -1) return;
    if (x == 0) gameloop();
  }
}