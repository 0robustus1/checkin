#include "database.h"

const char * DATABASE_FILE = "times.db";
const char * CONFIG_PATH   = ".config/checkin";

void open_db_connection(char * const db_file)
{
  if ( !*db_open )
  {
    sqlite3_open(db_file, &db_handler);
    *db_open = true;
  }
}

void close_db_connection()
{
  if( *db_open )
  {
    sqlite3_close(db_handler);
    *db_open = false;
  }
}

void initialize_database_connection()
{
  char * db_file = (char *) malloc( 120 * sizeof(char) );
  sprintf(db_file, "%s/%s/%s",getenv("HOME"),CONFIG_PATH,DATABASE_FILE);

  struct stat db_file_stat;
  if( stat(db_file, &db_file_stat) == -1 )
    checkin_create_database(db_file);

  open_db_connection(db_file);
  free(db_file);
}

void kill_database_connection()
{
  close_db_connection();
}

void create_table(sqlite3 *handle)
{
  sqlite3_exec(handle,
    "CREATE TABLE timeslots "
      "(id integer primary key autoincrement, "
      "begins text, "
      "ends text, "
      "description text "
      ");",
    NULL, NULL, NULL);
}

void checkin_create_database(char * const db_file)
{
  printf("Creating database...");
  open_db_connection(db_file);
  create_table(db_handler);
  close_db_connection();
  puts("successful");
}
