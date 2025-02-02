#ifndef DB_H
#define DB_H

#include <sqlite3.h>

extern sqlite3 *db;

void executeSql(const char* sql);
void initializeDatabase();
int usernameExists(const char *username);
int checkCredentials(const char *username, const char *password);
void signup(const char *username, const char *email, const char *password);
void forgetPassword(const char *username, const char *new_password);
void updateUserData(const char *username, const char *file_path, int score, int gold);
void displayUserData(const char *username);
void initDB();

int getScoreboard(char *name[], int score[], int gold[], int exp[], int result[]);
int getScoreboardForUser(const char *username, char *name[], int score[], int gold[], int exp[], int result[]);
int getAllMatches(const char *username, int score[], int gold[], int exp[], int result[]);
void addMatch(const char *username, int gold, int score, int exp, int result);

void userInfoDB(const char *username, int *score, int *gold, int *games, int *won, int *lost);



#endif