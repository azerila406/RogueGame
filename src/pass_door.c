#include "game.h"

const char *ERRORS[] = {
    "ERROR: Ghostly password! You've been caught by Inky! Try again...before I "
    "turn blue and eat you.",
    "ACCESS DENIED: Waka waka... wrong password! I'm out of pellets and "
    "patience. Try harder, or I'll use my power-up on YOU.",
    "INCORRECT INPUT: Looks like you've encountered the wrong maze. Try "
    "entering a path that makes sense! (Hint: It's not '12345').",
    "PASSWORD FAIL: You've been cornered! The ghosts are laughing. Next "
    "password, please!",
    "ERROR 404: Password not found in this dimension! Perhaps try a different "
    "quadrant?",
    "WRONG PASSWORD: You're trying to open the wrong power-up! This one's "
    "locked. Get the key (aka. The right password).",
    "ACCESS DENIED: This isn't a bonus level. Try the actual password.",
    "ERROR: Level failure. Your password attempt was a complete ghost. Try "
    "again... I'm getting hungry!",
    "INCORRECT ENTRY: That was like trying to eat a wall. No progress was "
    "made! Try an actual password this time.",
    "PASSWORD ERROR: Nope. That password was so bad, it made me burp. Try "
    "something a bit more edible.",
    "ACCESS DENIED: That password was like trying to eat a ghost... not gonna "
    "work. Try something... real.",
    "INCORRECT PASSWORD: Oh no! I just blew a fuse! Please use a real "
    "password, I don't want to reboot.",
    "PASSWORD FAIL: Sorry, that password is not part of my menu. Check the "
    "password sheet and try again.",
    "ERROR: This password is as tasty as a bad apple. Use a correct password "
    "please!",
    "ERROR: Did you even try? My ghost friends are judging your password "
    "attempt.",
    "ACCESS DENIED: Looks like someone got the wrong game. Try a legitimate "
    "password!"};
#define ERROR_SZ 16
const int ERROR_COLOR[3] = {1, 6, 3};

int passDoorLogin(Lock *lock) {
  char *menu[2] = {"Guess The Password", "Use Ancient Key"};
  char *msg[2] = {"You can try to guess the password to unlock the door....",
                  "If you have a key, you can use it..."};
  int x = createMenu(menu, msg, 2);
  if (x == -1) return 0;
  if (x == 0) {  // Guess Password
    char *pass = getInput("Enter Password:", 1);
    for (int i = 0; i < 3; i++) {
      if (checkPass(lock, pass)) {
        return 1;
      }
      renderMsgAndWait(ERRORS[rand() % ERROR_SZ], ERROR_COLOR[i]);
      lock->tried++;
    }
  } else if (x == 1) {  // TODO
    renderMsgAndWait("Ooops we've been too lazy to implement this...", 1);
  }
  return 0;
}

bool checkPass(Lock *lock, char *pass) {
  if (lock->tried >= 3) return 0;
  if (lock->time_based) {
    clock_t t = clock();
    if ((1.0 * t - lock->last_generated_pass) / CLOCKS_PER_SEC > DURATION_PASS)
      return 0;
  }
  if (strcmp(pass, lock->pass)) return 0;
  return 1;
}

void initLock(Lock *lock, bool time_based) {
  lock->last_generated_pass = clock();
  lock->time_based = time_based;
  lock->tried = 0;
  lock->pass = randomPass(4);
}