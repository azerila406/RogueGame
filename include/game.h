#ifndef GAME_H
#define GAME_H

#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS_PLAYER 100
#define MAX_ITEMS_TILE 100
#define HEIGHT 30
#define WIDTH 100
#define MIN_LEVELS 4
#define MAX_LEVELS 5
#define MAX_ROOMS_PER_LEVEL 7
#define MIN_ROOMS_PER_LEVEL 6
#define MIN_INTERIOR_OF_ROOM 6
#define MAX_INTERIOR_OF_ROOM 15
#define MIN_HALLWAY_LEN 10
#define HALLWAY_SIGHT 2
#define MSG_RESET_TIME 30

typedef struct DSU {
  int dpr[MAX_ROOMS_PER_LEVEL];
  int connected_component;
} DSU;

int gpr(DSU *, int);
void merge(DSU *, int, int);
void initDSU(DSU *, int);

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

extern Player *P;
extern int pr[HEIGHT][WIDTH][2], dist[HEIGHT][WIDTH], X[HEIGHT * WIDTH],
    Y[HEIGHT * WIDTH];
extern bool toggle_map_status;

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
  Room *room;
  int type;
  /*
  Type is floor, wall, door(!!)
  0 -> .
  1 -> vert wall && corner |
  2 -> corridor #
  3 -> horz wall -
  5 -> nothing
  6 -> door (normal door)
  8 -> upstairs
  10 -> downstairs
  12 -> hidden normal door |
  14 -> hidden normal door -
  16 -> secret generator &
  18 -> hidden secret door @
  */
} Tile;

typedef struct Level {
  int lvl_num;
  int num_room;
  Room room[MAX_ROOMS_PER_LEVEL];
  Tile tile[HEIGHT][WIDTH];
} Level;

typedef struct Game {
  Level lvl[MAX_LEVELS];
  int cur;
  int num_level;
  int pass;
} Game;

extern Game *G;

/* Screen Fucntions */
void initScreen();
void renderMsg(const char *string);
void renderMsgAndWait(const char *string);
void render(Level *level);
void renderHUD(Level *level);

/* Tile Fucntions*/
char tileChar(Tile *);
int whichRoomID(Level *level, int x, int y);

/* Player Functions*/
void initPlayer(Player *, Level *, int);  // AFTER LEVEL INIT
void movePlayer(Level *, int, int);
void moveStairs(Level *lvl, int where);
void processUnseen(Level *lvl);
void discoverTile(Level *lvl, int x, int y);

/* Room Functions*/
void initRoom(Level *, Room *);
int getX0(Room *room);
int getX1(Room *room);
int getY0(Room *room);
int getY1(Room *room);

/* Level Functions*/
void initLevel(Level *lvl);
void initLevelWithARoom(Level *lvl, Room *R);
void initHallway(Level *lvl);
void initRooms(Level *lvl);
void initRoomHallway(Level *lvl);

/* Game Fucntions */
void initGame();
void clearMsg();
void setTopMsg(char *str);
void checkTimerMsg();

/* Util Functions*/
char *to_string(char);
int rnd(int L, int R);
int max(int, int);
int min(int, int);
void rand_permute(int *a, int n);
char *randomPass(int n);

/* Game Logic */
void gameloop();
void processInput(int, Level *);

/* BFS functions*/
void BFS(Level *l, int sx, int sy, bool (*check)(Level *, int, int));
bool bfsDoor(Level *level, int sx, int sy, int roomID, bool rep);

/*MENU*/
bool checkInfo(const char *s);
char *getInput(const char *msg, bool is_echo);
int createMenu(char *s[], char *msg[], int n);
void renderMenu(char *s[], char *msg[], int n, int x);
void gameMenu();
void userMenu();

/* User management */
void userLogin();
void userRegister();
void userForgetPass();
void userScoreboard();

/* Check if the input is corrct */
bool isCorrectFormEmail(const char *email, bool error);
bool isCorrectFormPass(const char *password, bool error);
bool doesUserExists(const char *user, bool error);

#endif