#include "game.h"

int getX0(Room *r) { return r->x; }
int getX1(Room *r) { return r->x + r->height; }
int getY0(Room *r) { return r->y; }
int getY1(Room *r) { return r->y + r->width; }