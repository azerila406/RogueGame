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
    sql = "CREATE TABLE IF NOT EXISTS matches ("
                                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                    "user_id INTEGER NOT NULL,"
                                    "gold INTEGER NOT NULL,"
                                    "score INTEGER NOT NULL,"
                                    "experiences INTEGER NOT NULL,"
                                    "result INTEGER NOT NULL," // 1 for win, 0 for loss
                                    "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
                                    "FOREIGN KEY(user_id) REFERENCES users(id));";
    executeSql(sql);
}

void userInfoDB(const char *username, int *score, int *gold, int *games, int *won, int *lost, char last_match_time[]) {
    char sql[512];
    sqlite3_stmt *stmt;

    sprintf(sql, "SELECT score, gold, timestamp FROM matches WHERE user_id = (SELECT id FROM users WHERE username = '%s') ORDER BY score DESC LIMIT 1;", username);
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    *score = 0;
    *gold = 0;
    last_match_time[0] = '\0';
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *score = sqlite3_column_int(stmt, 0);
        *gold = sqlite3_column_int(stmt, 1);
        const char *timestamp = (const char *)sqlite3_column_text(stmt, 2);
        assert(timestamp);
        if (timestamp) {
            strcpy(last_match_time, timestamp);
        }
    }
    sqlite3_finalize(stmt);

    sprintf(sql, "SELECT COUNT(*), SUM(result), COUNT(*) - SUM(result) FROM matches WHERE user_id = (SELECT id FROM users WHERE username = '%s');", username);
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    *games = 0;
    *won = 0;
    *lost = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *games = sqlite3_column_int(stmt, 0);
        *won = sqlite3_column_int(stmt, 1);
        *lost = sqlite3_column_int(stmt, 2);
    }
    sqlite3_finalize(stmt);
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

void addMatch(const char *username, int gold, int score, int exp, int result) {
    char sql[256];
    sprintf(sql, "SELECT id FROM users WHERE username='%s';", username);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 0);

        sprintf(sql, "INSERT INTO matches (user_id, gold, score, experiences, result) VALUES (%d, %d, %d, %d, %d);",
                user_id, gold, score, exp, result);
        executeSql(sql);
    }

    sqlite3_finalize(stmt);
}

int getScoreboardForUser(const char *username, char *name[], int score[], int gold[], int exp[], int result[]) {
    const char *sql = "SELECT users.username, matches.score, matches.gold, matches.experiences, matches.result "
                      "FROM matches "
                      "JOIN users ON matches.user_id = users.id "
                      "WHERE users.username = ? "
                      "ORDER BY matches.score DESC, matches.gold DESC;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    int n = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *usr = (const char *)sqlite3_column_text(stmt, 0);
        name[n] = (char *) malloc(strlen(usr) * sizeof(char) + 1);
        strcpy(name[n], usr);
        score[n] = sqlite3_column_int(stmt, 1);
        gold[n] = sqlite3_column_int(stmt, 2);
        exp[n] = sqlite3_column_int(stmt, 3);
        result[n] = sqlite3_column_int(stmt, 4);
        ++n;
    }

    sqlite3_finalize(stmt);
    return n;
}

int getScoreboard(char *name[], int score[], int gold[], int exp[], int result[]) {
    const char *sql = "SELECT users.username, "
                      "SUM(matches.score) AS total_score, "
                      "SUM(matches.gold) AS total_gold, "
                      "SUM(matches.experiences) AS total_exp, "
                      "COUNT(matches.user_id) AS total_games "
                      "FROM matches "
                      "JOIN users ON matches.user_id = users.id "
                      "GROUP BY users.username "
                      "ORDER BY total_score DESC, total_gold DESC;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    int n = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *usr = (const char *)sqlite3_column_text(stmt, 0);
        name[n] = (char *) malloc((strlen(usr) + 1) * sizeof(char));
        strcpy(name[n], usr);
        score[n] = sqlite3_column_int(stmt, 1);
        gold[n] = sqlite3_column_int(stmt, 2);
        exp[n] = sqlite3_column_int(stmt, 3);
        result[n] = sqlite3_column_int(stmt, 4);
        ++n;
    }
    sqlite3_finalize(stmt);
    return n;
}

int getAllMatches(const char *username, int score[], int gold[], int exp[], int result[]) {
    char sql[256];
    sprintf(sql, "SELECT matches.score, matches.gold, matches.experiences, matches.result "
                 "FROM matches "
                 "JOIN users ON matches.user_id = users.id "
                 "WHERE users.username='%s' "
                 "ORDER BY matches.score DESC, matches.gold DESC;", username);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    int n = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        score[n] = sqlite3_column_int(stmt, 0);
        gold[n] = sqlite3_column_int(stmt, 1);
        exp[n] = sqlite3_column_int(stmt, 2);
        result[n] = sqlite3_column_int(stmt, 3); // Add result to the output
        ++n;
    }
    sqlite3_finalize(stmt);

    return n;
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

void createGuest() {
    if (!usernameExists("GUEST")) {
        signup("GUEST", "g@e.c", "");
    }
}

void initDB() {
    int rc = sqlite3_open("rogue-db.db", &db);
    initializeDatabase();
    createGuest();
}