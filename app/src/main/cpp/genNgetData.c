//
// Created by Abraham Bongcher on 17/05/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <jni.h>
#include <android/log.h>

#define TAG "sqlite"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)

static int callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    LOGI("%s: ", (const char *) data);
    for (i = 0; i < argc; i++) {
        LOGI("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}


jboolean JNICALL
Java_com_example_sqlitetest_MainActivity_generateDatabase(JNIEnv *env, jobject thiz) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("/mnt/sdcard/download/test.db", &db);

    if (rc) {
        LOGE("Can't open database: %s", sqlite3_errmsg(db));
        return (0);
    } else {
        LOGI("Opened database successfully");
    }

    /* Create SQL statement */
    sql = "CREATE TABLE COMPANY("  \
            "ID INT PRIMARY KEY     NOT NULL," \
            "NAME           TEXT    NOT NULL," \
            "AGE            INT     NOT NULL," \
            "ADDRESS        CHAR(50)," \
            "SALARY         REAL );"\
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );" \
            "SELECT * from COMPANY";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *) data, &zErrMsg);

    if (rc != SQLITE_OK) {
        LOGE("SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        LOGI("Records created successfully");
    }
    sqlite3_close(db);
    return 0;
}


