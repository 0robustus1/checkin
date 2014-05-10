#ifndef DATABASE_H_OCCURED
#define DATABASE_H_OCCURED

#include <sqlite3.h>
#include "common.h"

extern const char * DATABASE_FILE;
extern const char * CONFIG_PATH;


sqlite3 *db_handler;
int *db_open;

void initialize_database_connection();
void kill_database_connection();
void open_db_connection(char * const db_file);
void close_db_connection();
void create_table(sqlite3 *handle);
void checkin_create_database(char * const db_file);

#endif /* DATABASE_H_OCCURED */
