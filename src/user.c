#include "game.h"

bool checkInfo(const char *s) {
  clear();
  renderMsg(s, 1);
  int ch;
  while (1) {
    ch = getch();
    switch (ch) {
      case 'n':
      case 'N':
        return 0;
      case 'y':
      case 'Y':
        return 1;
    }
  }
}

bool confirmInfo(const char *s) {
  char t[200];
  sprintf(t, "Do you confirm ? %s (y/n)", s);
  return checkInfo(t);
}

bool isUserCorrect(const char *user, const char *pass) { return 1; }  // TODO

void userLogin() {
  char *user, *pass;
  do {
    user = getInput("Enter Your Username: ", 1);
    pass = getInput("Enter Your Password: ", 0);
  } while (!isUserCorrect(user, pass));
  clear();
  renderMsg("Logged in Successfully...", 1);
  getch();
  gameMenu();
}
void userRegister() {
  char *user;
  do {
    user = getInput("Enter Your Username: ", 1);
  } while (doesUserExists(user, 1) || !confirmInfo(user));

  char *email;
  do {
    email = getInput("Enter Your E-Mail: ", 1);
  } while (!isCorrectFormEmail(email, 1) || !confirmInfo(email));

  char *pass, *pass2;
  if (checkInfo("Do you want randomly generated password? (y/n)")) {
    do {
      pass = randomPass(rnd(9, 20));
    } while (!confirmInfo(pass));
  } else {
    do {
      pass = getInput("Enter Your Password: ", 0);
      pass2 = getInput("Repeat Your Password: ", 0);
    } while (!isCorrectFormPass(pass, 1) || strcmp(pass, pass2));
  }
  clear();
  renderMsg("Registered Succesfully... Log in to continue", 1);
  getch();
}
void userForgetPass() {}
void userScoreboard() {}