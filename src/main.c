#include "game.h"

// Array of strings
const char *GAME_OVER_ERROR[] = {
    "Well, that was tragic. Maybe try using your hands next time?",
    "Congratulations! You've unlocked the 'Professional Floor Inspector' achievement.",
    "RIP. Rest in Pieces. (You were *this* close... to being good.)",
    "You died! But hey, at least you made the enemies feel better about themselves.",
    "Game Over! Want to try again, or is this your final form?",
    "You've been defeated! Insert more coins to continue... or just give up and take up knitting.",
    "Oops! Did you forget how to [insert basic game mechanic]?",
    "You died! On the bright side, your tombstone will read: 'Tried their best (but their best was bad).'",
    "Well, that was embarrassing. Want to try again, or should we call your mom?",
    "You've been vanquished! Maybe next time, try not to walk into the obvious danger?",
    "Game Over! Don't worry, even legends like you... wait, no, never mind. You're not a legend.",
    "You died! But hey, at least you're consistent... at failing.",
    "You've met your demise! Want to try again, or should we start writing your obituary?",
    "You died! Pro tip: Breathing helps. So does not dying.",
    "Game Over! You've reached peak 'oops' energy. Try again?",
    "You died! But don't worry, the enemies will miss you... because you were so easy to beat.",
    "You've been defeated! Maybe next time, try using your brain instead of your face to block attacks.",
    "Game Over! You've unlocked the 'Participation Trophy' achievement. Yay?",
    "You died! But hey, at least you're setting a great example of what *not* to do.",
    "You've been vanquished! Want to try again, or should we just uninstall the game for you?"};
#define GAME_OVER_ERROR_SZ 20

Game *G;
Player *P;

int game_movement_timer = 0;

void gameOver() {
  renderMsgAndWait(GAME_OVER_ERROR[rand() % GAME_OVER_ERROR_SZ], 3);
  char *s = (char *) malloc(100 * sizeof(char));
  sprintf(s, "Gold: %d    Score: %d    ", P->gold, P->score);
  renderMsgAndWait(s, 2);
  //save the result
  //TODO call scoreboard here :)
}

void gameloop() {
  clear();

  initGame();

  while (1) {
    checkTimerMsg();
    processPlayer();
    // if (P->health <= 0) return gameOver(); FOR DEBUG PURPOSES

    if (get_game_timer() % P->speed_mult == 0) {
      //processEnemies :))
    }

    processUnseen(&(G->lvl[G->cur]));
    render(&(G->lvl[G->cur]));
    int ch = getch();
    if (ch == 'q') return;
    processInput(ch, &(G->lvl[G->cur]));
    game_movement_timer++;
  }
}

int get_game_timer() {
  return game_movement_timer;
}

signed main() {
  setlocale(LC_ALL, "");
  srand(time(0));
  initScreen();
  gameMenu();
  // userMenu();
  //  gameloop();
  endwin();
  return 0;
}