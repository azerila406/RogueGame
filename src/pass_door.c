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

const char *locked_msg[] = {
    "Too many tries. Are you sure you've used a computer before?",
    "Locked out. Maybe write your password down next time?",
    "You've failed more times than a toddler trying to use chopsticks.",
    "Too many attempts. Are you just mashing the keyboard at this point?",
    "Locked out. Maybe ask your goldfish for help—it probably remembers your "
    "password.",
    "You've been locked out. Maybe take up a new hobby? Like memory training?",
    "Too many tries. Are you even *trying* to remember, or just hoping for a "
    "miracle?",
    "Locked out. Maybe your password is '123456'? Oh wait, you already tried "
    "that.",
    "Too many failed attempts. Are you sure you're not a bot? Because you're "
    "acting like one.",
    "Locked out. Maybe try using your brain instead of your luck next time.",
    "Too many tries. Did you forget your password or your entire identity?",
    "Locked out. Maybe call your mom—she probably remembers your password.",
    "Too many attempts. Are you trying to log in or brute-force your way in?",
    "Locked out. Maybe take a break and rethink your life choices.",
    "Too many tries. Are you sure you're not just typing 'password' over and "
    "over?",
    "Locked out. Maybe try using your other brain cell this time.",
    "Too many failed attempts. Are you even human? Prove it by solving a "
    "captcha.",
    "Locked out. Maybe your password is 'incorrect.' Oh wait, you already "
    "tried that.",
    "Too many tries. Are you sure you're not just guessing randomly?",
    "Locked out. Maybe try using your memory instead of your hope.",
    "Too many attempts. Maybe it's time to admit you're not getting in today.",
    "Locked out. Maybe try turning your brain on and off again.",
    "Too many tries. Are you sure you're not just typing your Wi-Fi password?",
    "Locked out. Maybe try using your password instead of your excuses.",
    "Too many attempts. Are you sure you're not just typing 'letmein' "
    "repeatedly?"};
#define LOCKED_SZ 25

const int ERROR_COLOR[3] = {1, 6, 3};

int passDoorLogin(Lock *lock) {
  if (lock->tried >= 3) {
    renderMsgAndWait(locked_msg[rand() % LOCKED_SZ], ERROR_COLOR[2]);
    return 0;
  }
  char *menu[2] = {"Guess The Password", "Use Ancient Key"};
  char *msg[2] = {"You can try to guess the password to unlock the door....",
                  "If you have a key, you can use it..."};
  int x = createMenu(menu, msg, "Lock Menu", 2);
  if (x == -1) return 0;
  if (x == 0) {  // Guess Password
      char *pass = getInput("Enter Password:", 1);
      if (checkPass(lock, pass)) {
        return 1;
      renderMsgAndWait(ERRORS[rand() % ERROR_SZ], ERROR_COLOR[lock->tried++]);
      return 0;
    }
  } else if (x == 1) {  // TODO
    renderMsgAndWait("Ooops we've been too lazy to implement this... so we will let you pass", 3);
    return 1;
  }
  return 0;
}

bool checkPass(Lock *lock, char *pass) {
  if (lock->tried >= 3) return 0;
  if (lock->time_based) {
    if (difftime(time(NULL), lock->last_generated_pass) >= DURATION_PASS)
      return 0;
  }
  if (strcmp(pass, lock->pass)) return 0;
  return 1;
}

void initLock(Lock *lock, bool time_based) {
  lock->last_generated_pass = time(NULL);
  lock->time_based = time_based;
  lock->tried = 0;
  lock->pass = randomPass(4);
}

void reinitLock(Lock *lock) {
  lock->last_generated_pass = time(NULL);
  lock->pass = randomPass(4);
  setTopMsg(lock->pass);
}