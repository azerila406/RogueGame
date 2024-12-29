#include "game.h"

char tileChar(Tile *t) {
  switch (t->type) {
    case 0:
      return '.';
    case 1:
      return '|';
    case 3:
      return '-';
    case 5:
      return ' ';
    default:
      exit(5);
      return 0;
  }
}