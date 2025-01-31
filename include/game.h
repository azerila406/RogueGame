#ifndef GAME_H
#define GAME_H

#include <assert.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>

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
#define MSG_RESET_TIME 5
#define DURATION_PASS 30
#define MAX_FOOD 100
#define MAX_CURSE 100
#define MAX_WEAPON 5
#define MAX_HUNGER 20
#define MAX_ENEMEY 300
#define TIME_OF_DEGRADING_OF_FOOD 30
#define TIME_OF_DAMAGE_MULT_LASTING 15
#define TIME_OF_SPEED_MULT_LASTING 15
#define TIME_OF_HEALTH_RECOVER_MULT_LASTING 15
#define TIME_OF_HUNGER_DECREASE 5

extern int DIFF_LEVEL;
extern int MAIN_COLOR;

typedef struct DSU {
  int dpr[MAX_ROOMS_PER_LEVEL];
  int connected_component;
} DSU;

int gpr(DSU *, int);
void merge(DSU *, int, int);
void initDSU(DSU *, int);

typedef struct Food {
  int type;
  // 0 -> Normal Food
  // 1 -> Power Food
  // 2 -> Speed Food
  // 3 -> Bad Food
  int pickup_time;
} Food;

typedef struct Gold {
  int gold;
  int type;
} Gold;

typedef struct Curse {
  int type;
} Curse;

typedef struct Weapon {
  int type;
  int td;
  int range;
  int dmg;
  // Mace ⚒ 0 -> short range
  // Dagger 🗡 1
  // Magic Wand 🪄 2
  // Normal Arrow ➳ 3
  // Sword ⚔ 4 -> short range
} Weapon;

extern char *WEAPON_NAME_BY_TYPE[];
extern char *FOOD_NAME_BY_TYPE[];
extern char *CURSE_NAME_BY_TYPE[];
extern int WEAPON_DAMAGE_BY_TYPE[];
extern int WEAPON_RANGE_BY_TYPE[];
extern int WEAPON_TD_BY_TYPE[];
extern char *username;


typedef struct Player {
  int x;
  int y;
  int health;
  int max_health;
  int gold;
  int exp;
  int def_weapon;
  int num_food;
  int num_curse;
  int score;
  Food *food[MAX_FOOD];
  Weapon weapon[MAX_WEAPON];
  Curse *curse[MAX_CURSE];

  int damage_mult_last_time;
  int damage_mult;

  int hunger;
  int last_time_hunger;

  int speed_mult;
  int speed_mult_last_time;

  int health_recover_mult;
  int health_recover_mult_last_time;

} Player;

typedef struct Vector {
  int *a;
  int sz;
} Vector;

void vec_push(Vector *V, int x);
void vec_init(Vector *V);
void vec_pop(Vector *V);
void vec_free(Vector *V);
void vec_test();

extern Player *P;
extern int pr[HEIGHT][WIDTH][2], dist[HEIGHT][WIDTH], X[HEIGHT * WIDTH],
    Y[HEIGHT * WIDTH];
extern bool toggle_map_status;
extern int GOD_MODE;

typedef struct Lock {
  char *pass;
  int tried;
  time_t last_generated_pass;
  bool time_based;
} Lock;

typedef struct Room {
  int x;
  int y;
  int height;
  int width;
  bool visible;
  int type;
  //Normal Room -> 0 (50%)
  //Treasure Room -> 1 (only last floor (game finishes on this room when defeat all enemies))
  //Enchant Room -> 2 (only accessable by stairs, no enemy, lots of curse and food)
  //Nightmare Room -> 3 (no gold, no food, low sight)
} Room;

typedef struct Tile {
  // Enemies
  int num_item;
  bool visible;
  Lock *lock;
  int type;
  /*
  Type is floor, wall, door(!!)
  0 -> .
  1 -> vert wall && corner |
  2 -> corridor #
  3 -> horz wall -
  21 -> O (sotoon)
  5 -> nothing
  6 -> door (normal door)
  22 -> trap
  8 -> upstairs
  10 -> downstairs
  12 -> hidden normal door |
  14 -> hidden normal door -
  16 -> pass generator &
  18 -> pass door @ (locked)
  20 -> pass door @ (unlocked)
  42 -> Item!
  */
  int room_type;
  Gold *G;
  Food *F;
  Curse *C;
  Weapon *W;

} Tile;

typedef struct Enemy {
  int x;
  int y;
  int type;
  //0 -> Deamon
  //1 -> Fire
  //2 -> Giant
  //3 -> Snake
  //4 -> Undeed
  int speed;
  int health;
  int score;
  int last_time_attacked;
} Enemy;

typedef struct Level {
  int lvl_num;
  int num_room;
  Room room[MAX_ROOMS_PER_LEVEL];
  Tile tile[HEIGHT][WIDTH];
  int num_enemy;
  Enemy enemy[MAX_ENEMEY];
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
void renderMsg(const char *string, int color);
void renderMsgAndWait(const char *string, int color);
void render(Level *level);
void renderHUD(Level *level);

/* Tile Fucntions*/
wchar_t *tileChar(Tile *);
int whichRoomID(Level *level, int x, int y);

/* Player Functions*/
void initPlayer(Player *, Level *, int);  // AFTER LEVEL INIT
void movePlayer(Level *, int, int);
void moveStairs(Level *lvl, int where);
void processUnseen(Level *lvl);
void discoverTile(Level *lvl, int x, int y);
void processPlayer();

/* Room Functions*/
void initRoom(Level *, Room *);
int getX0(Room *room);
int getX1(Room *room);
int getY0(Room *room);
int getY1(Room *room);
int isThereStairs(Level *lvl, Room *room);

/* Level Functions*/
void initLevel(Level *lvl);
void initLevelWithARoom(Level *lvl, Room *R);
void initHallway(Level *lvl);
void initRooms(Level *lvl);
void initRoomHallway(Level *lvl);
void initItemsLevel(Level *lvl);
void initItemsRoom(Level *lvl, Room *room);
void initRoomsType(Level *lvl);
void expandTypeToAllTiles(Level *lvl, Room *room);


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
int sgn(int x);
int myabs(int x);

/* Game Logic */
void gameloop();
int get_game_timer();
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

/* Pass Door */
int passDoorLogin(Lock *L);
bool checkPass(Lock *L, char *pass);
void initLock(Lock *L, bool time_based);
void reinitLock(Lock *L);
void makePassDoor(Level *L, int room);

/* Item Management */
void discoverItem(Tile *T);
void searchItem(Tile *T);
void showWeapon();
void showInventory();
void processFoodOverTime();

/* Enemy Stuff */
void attack(Level *lvl);
void initEnemies(Level *lvl);
void initEnemy(Enemy *enemy);
void processEnemies(Level *lvl);

#endif