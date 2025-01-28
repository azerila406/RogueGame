#include "game.h"

char *getInput(const char *msg, bool enable_echo) {
  clear();
  renderMsg(msg, 1);
  char *s = (char *)malloc(100 * sizeof(char));
  if (enable_echo) echo();
  getnstr(s, 99);
  if (enable_echo) noecho();
  return s;
}

void renderMenu(char *s[], char *msg[], int n, int x) {
  clear();
  renderMsg(msg[x], 1);

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

void userMenu() {
  char *s[4] = {"Login", "Register", "Forget Password", "Scoreboard"};
  char *msg[4] = {"Login using username and password", "Is this your first time entering the game? then signup", "Change password in case you forgot", "Show the scoreboard"};

  while (1) {
    int x = createMenu(s, msg, 4);
    switch (x) {
      case -1:
        return;
      case 0:
        userLogin();
        continue;
      case 1:
        userRegister();
        continue;
      case 2:
        userForgetPass();
        continue;
      case 3:
        userScoreboard();
        continue;
    }
  }
}