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
                      "password TEXT NOT NULL);";
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

void signup(const char *username, const char *email, const char *password) {
    char sql[256];
    sprintf(sql, "INSERT INTO users (username, email, password) VALUES ('%s', '%s', '%s');",
             username, email, password);
    executeSql(sql);
}

void forgetPassword(const char *username, const char *new_password) {
    char sql[256];
    sprintf(sql, "UPDATE users SET password='%s' WHERE username='%s';",
             new_password, username);
    executeSql(sql);
}

void initDB() {
    int rc;
    rc = sqlite3_open("rogue-db.db", &db);
    if (rc != SQLITE_OK) {
        assert(0);
    }
    initializeDatabase();
}