#ifndef GAME_H
#define GAME_H

#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITEMS_PLAYER 100
#define MAX_ITEMS_TILE 100
#define MAX_ROOMS 6
#define HEIGHT 30
#define WIDTH 100
#define MAX_LEVELS 4
#define MAX_ROOMS_PER_LEVEL 7
#define MIN_ROOMS_PER_LEVEL 6
#define MIN_INTERIOR_OF_ROOM 6
#define MAX_INTERIOR_OF_ROOM 15

typedef struct DSU {
  int dpr[MAX_ROOMS_PER_LEVEL];
  int connected_component;
} DSU;

int gpr(DSU*, int);
void merge(DSU*, int, int);
void init_dsu(DSU*);

typedef struct Item {
} Item;

typedef struct Player {
  int x;
  int y;
  int health;
  int max_health;
  int gold;
  int exp;
  int num_item;
  Item items[MAX_ITEMS_PLAYER];
} Player;

extern Player* P;

typedef struct Room {
  int type;
  /*type = 0 -> Normal Room*/
  int x;
  int y;
  int height;
  int width;
  bool visible;
} Room;

typedef struct Tile {
  // Enemies
  int num_item;
  Item items[MAX_ITEMS_TILE];
  bool visible;
  Room* room;
  int type;
  /*
  Type is floor, wall, door(?)
  1 -> vert wall && corner |
  3 -> horz wall -
  5 -> nothing
  */
} Tile;

typedef struct Level {
  int lvl_num;
  int num_room;
  Room room[MAX_ROOMS];
  Tile tile[HEIGHT][WIDTH];
} Level;

typedef struct Game {
  Level lvl[MAX_LEVELS];
} Game;

/* Screen Fucntions */
void initScreen();
void render(Level*);
void renderHUD(Level*);

/* Tile Fucntions*/
char tileChar(Tile*);
int whichRoomID(Level*, int, int);

/* Player Functions*/
void initPlayer(Player*, Level*, int);  // AFTER LEVEL INIT
void movePlayer(Level*, int, int);

/* Room Functions*/
void initRoom(Level*, Room*);

/* Level Functions*/
void initLevel(Level*);

/* Game Fucntions */
void initGame(Game*);

/* Util Functions*/
char* to_string(char);
int rnd(int, int);
int max(int, int);
int min(int, int);

/* Game Logic */
void gameloop();
void processInput(int, Level*);

#endif