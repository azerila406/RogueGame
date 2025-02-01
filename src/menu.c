#include "game.h"
#include "database.h"


char *getInput(const char *msg, bool enable_echo)
{
  clear();
  renderMsg(msg, 1);
  char *s = (char *)malloc(100 * sizeof(char));
  if (enable_echo)
    echo();
  getnstr(s, 99);
  if (enable_echo)
    noecho();
  return s;
}

void renderMenu(char *s[], char *msg[], char *menu_name, int n, int x)
{
    clear();
    refresh();

    int w = 0;
    for (int i = 0; i < n; i++) {
        w = max(w, strlen(s[i]));
    }
    w += 6;
    int h = n + 4;

    int startX = (COLS - w) / 2;
    int startY = (LINES - h) / 2;
    WINDOW *win = newwin(h, w, startY, startX);
    box(win, 0, 0);

    wattron(win, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(win, 1, (w - strlen(menu_name)) / 2, menu_name);
    wattroff(win, COLOR_PAIR(1) | A_BOLD);

    renderMsg(msg[x], 1);

    for (int i = 0; i < n; i++) {
        if (i == x)
            wattron(win, A_REVERSE | A_BOLD);
        mvwprintw(win, i + 3, 3, "%s", s[i]);
        if (i == x)
            wattroff(win, A_REVERSE | A_BOLD);
    }

    wrefresh(win);
    refresh();
    move(startY + x + 3, startX + 3);
    delwin(win);
}

int createMenu(char *s[], char *msg[], char *menu_name, int n)
{
  if (n == 0) assert(0);
  clear();
  int i = 0, ch;
  int L = 0;
  do
  {
    renderMenu(s + L, msg + L, menu_name, min(MAX_ENTRY_IN_MENU, n), i - L);
    ch = getch();
    switch (ch)
    {
    case KEY_UP:
    case 'w':
    case 'W':
      if (i == 0) continue;
      if (i == L) --L;
      --i;
      continue;
    case KEY_DOWN:
    case 's':
    case 'S':
      if (i == n - 1) continue;
      if (i - L + 1 == MAX_ENTRY_IN_MENU) L++;
      i++;  
      continue;
    case '\n':
    case '\r':
      return i;
    }
  } while (ch != 'q' && ch != 'Q');
  return -1;
}

void diffMenu() {
  char *s[4] = {"Easy", "Med", "Hard", "Impossible"};
  char *msg[4] = {"For begginers", "For intermediates", "For exprienced players", "Darksouls"};
  int x = createMenu(s, msg, "Difficulty Settings", 4);
  if (x == -1) return;
  DIFF_LEVEL = x;
}

void changeMainCharColor() {
  char *s[] = {"Yellow", "White", "Red", "Cyan", "Green"};
  char *msg[] = {"", "", "", "", ":)"};
  int x = createMenu(s, msg, "Color Settings", 5);
  if (x == -1) return;
  MAIN_COLOR = x;  
}

void userInfo() {
  int score[MAX_ENTRY], gold[MAX_ENTRY], result[MAX_ENTRY], exp[MAX_ENTRY];
  int n = getAllMatches(username, score, gold, exp, result);
  if (n == 0) {
    renderMsgAndWait("You have no info currently", 1);
    return;
  }
  //TODO some background on the user :))
  char *s[MAX_ENTRY], *msg[MAX_ENTRY];
  for (int i = 0; i < n; i++) {
    char *x = (char *) malloc(500 * sizeof(char));
    sprintf(x, "Score: %d   Gold: %d   Exp: %d   Game Status: %s", score[i], gold[i], exp[i], (result[i] ? "WON" : "LOST"));
    s[i] = x;
    msg[i] = "";
  }
  //createMenu(s, msg, n);
  //TODO
}

void settingMenu()
{
  char *s[4] = {"Difficulty", "Main Character Color", "Music", "GOD Mode"};
  char *msg[4] = {"Change hardness", "Change Color", "Music settings", "You won't die"};
  int x = createMenu(s, msg, "Settings", 4);
  switch (x)
  {
  case -1:
    return;
  case 0:
    diffMenu();
    return;
  case 1:
    changeMainCharColor();
    return;
  case 2:
    renderMsgAndWait("currently no music!", 1);
    return;
  case 3:
    GOD_MODE = 1;
    return;
  }
}

void gameMenu()
{
  char *s[4] = {"New Game", "Continue Game", "Settings", "User Info"};
  char *msg[4] = {"Starts a New Game", "Continues previous game if exists", "Change Settings", "Check your info"};
  while (1)
  {
    int x = createMenu(s, msg, "Game Menu", 4);
    // TODO for now
    switch (x)
    {
    case -1:
      return;
    case 0:
      initGame();
      gameloop();
      break;
    case 1:
      resumeGame();
      break;
    case 2:
      settingMenu();
      break;
    case 3:
      userInfo();
      break;
    }
  }
}

void userMenu()
{
  char *s[4] = {"Login", "Register", "Forget Password", "Scoreboard"};
  char *msg[4] = {"Login using username and password", "Is this your first time entering the game? then signup", "Change password in case you forgot", "Show the scoreboard"};

  while (1)
  {
    int x = createMenu(s, msg, "Main Menu", 4);
    switch (x)
    {
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