#include "game.h"
#include "database.h"

sqlite3* db;

void executeSql(const char *sql) {
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
}

void initializeDatabase() {
    const char *sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT NOT NULL UNIQUE,"
                      "email TEXT NOT NULL UNIQUE,"
                      "password TEXT NOT NULL,"
                      "score INTEGER DEFAULT 0,"
                      "gold INTEGER DEFAULT 0,"
                      "file_path TEXT);";
    executeSql(sql);
}

int usernameExists(const char *username) {
    char sql[256];
    sprintf(sql, "SELECT username FROM users WHERE username='%s';", username);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        assert(0);
        return 0;
    }

    int exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

int checkCredentials(const char *username, const char *password) {
    char sql[256];
    sprintf(sql, "SELECT username FROM users WHERE username='%s' AND password='%s';", username, password);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        assert(0);
        return 0;
    }

    int valid = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return valid;
}

void getScoreboard() {
    const char *sql = "SELECT username, score FROM users ORDER BY score DESC;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        assert(0);
        return;
    }

    printf("Scoreboard:\n");
    printf("Username\tScore\n");
    printf("----------------------\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *username = (const char *)sqlite3_column_text(stmt, 0);
        int score = sqlite3_column_int(stmt, 1);
        printf("%s\t\t%d\n", username, score);
    }

    sqlite3_finalize(stmt);
}

void signup(const char *username, const char *email, const char *password) {
    if (usernameExists(username)) {
        printf("Username '%s' already exists.\n", username);
        return;
    }

    char sql[256];
    sprintf(sql, "INSERT INTO users (username, email, password) VALUES ('%s', '%s', '%s');",
             username, email, password);
    executeSql(sql);
    printf("User '%s' signed up successfully!\n", username);
}

void forgetPassword(const char *username, const char *new_password) {
    if (!usernameExists(username)) {
        printf("Username '%s' does not exist.\n", username);
        return;
    }

    char sql[256];
    sprintf(sql, "UPDATE users SET password='%s' WHERE username='%s';",
             new_password, username);
    executeSql(sql);
    printf("Password for user '%s' has been reset.\n", username);
}

void updateUserData(const char *username, const char *file_path, int score, int gold) {
    if (!usernameExists(username)) {
        printf("Username '%s' does not exist.\n", username);
        return;
    }

    char sql[256];
    sprintf(sql, "UPDATE users SET file_path='%s', score=%d, gold=%d WHERE username='%s';",
             file_path, score, gold, username);
    executeSql(sql);
    printf("User data for '%s' updated successfully!\n", username);
}

void displayUserData(const char *username) {
    char sql[256];
    sprintf(sql, "SELECT * FROM users WHERE username='%s';", username);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        assert(0);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("User Data:\n");
        printf("ID: %d\n", sqlite3_column_int(stmt, 0));
        printf("Username: %s\n", sqlite3_column_text(stmt, 1));
        printf("Email: %s\n", sqlite3_column_text(stmt, 2));
        printf("Password: %s\n", sqlite3_column_text(stmt, 3));
        printf("Score: %d\n", sqlite3_column_int(stmt, 4));
        printf("Gold: %d\n", sqlite3_column_int(stmt, 5));
        printf("File Path: %s\n", sqlite3_column_text(stmt, 6));
    } else {
        printf("User '%s' not found.\n", username);
    }

    sqlite3_finalize(stmt);
}

void initDB() {
    int rc;
    rc = sqlite3_open("rogue-db.db", &db);
    if (rc != SQLITE_OK) {
        assert(0);
    }
    initializeDatabase();
}