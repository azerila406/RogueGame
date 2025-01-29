#include "game.h"

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

void renderMenu(char *s[], char *msg[], int n, int x)
{
  clear();
  renderMsg(msg[x], 1);

  for (int i = 0; i < n; i++)
  {
    if (i == x)
      attron(A_REVERSE | A_BOLD);
    mvprintw(i + 2, 2, s[i]);
    if (i == x)
      attroff(A_BOLD | A_REVERSE);
  }
  move(x + 2, 2);
}

int createMenu(char *s[], char *msg[], int n)
{
  clear();
  int i = 0, ch;
  do
  {
    renderMenu(s, msg, n, i);
    ch = getch();
    switch (ch)
    {
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

void diffMenu() {
  char *s[4] = {"Easy", "Med", "Hard", "Impossible"};
  char *msg[4] = {"For begginers", "For intermediates", "For exprienced players", "Darksouls"};
  int x = createMenu(s, msg, 4);
  if (x == -1) return;
  DIFF_LEVEL = x;
}

void changeMainCharColor() {
  char *s[] = {"Yellow", "White", "Red", "Cyan", "Green"};
  char *msg[] = {"", "", "", "", ":)"};
  int x = createMenu(s, msg, 5);
  if (x == -1) return;
  MAIN_COLOR = x;  
}

void userInfo() {
  if (!strcmp("GUEST", username)) {
    renderMsgAndWait("You are logged in as a guest therefore no info!", 1);
    return;
  }
  //TODO some background on the user :))
}

void settingMenu()
{
  char *s[3] = {"Difficulty", "Main Character Color", "Music"};
  char *msg[3] = {"Change hardness", "Change Color", "Music settings"};
  int x = createMenu(s, msg, 3);
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
  }
}

void gameMenu()
{
  char *s[4] = {"New Game", "Continue Game", "Settings", "User Info"};
  char *msg[4] = {"Starts a New Game", "Continues previous game if exists", "Change Settings", "Check your info"};
  while (1)
  {
    int x = createMenu(s, msg, 4);
    // TODO for now
    switch (x)
    {
    case -1:
      return;
    case 0:
      gameloop();
      break;
    case 1:
      renderMsgAndWait("We've been too lazy", 1);
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
    int x = createMenu(s, msg, 4);
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