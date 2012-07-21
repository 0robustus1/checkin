#include "checkin.h"

const char * DATABASE_FILE = "times.db";
const char * TIME_FORMAT = "%Y-%m-%d %H:%M:%S";
int main(int argc, char*argv[])
{
  int id;
  int entries = 0;
  sqlite3 *db_handler;
  sqlite3_open(DATABASE_FILE, &db_handler);
  struct Timeslot *timeslots;
  timeslots = read_entries(db_handler, &entries);
  if (timeslots == NULL) 
  {
    printf("Problem while parsing...");
    return 1;
  }
  /*int i;*/
  /*for(i=0;i<entries;i++)*/
    /*show_timeslot(timeslots + (i*sizeof(struct Timeslot)));*/
  print_month(timeslots, entries, 2012, 7);
  sqlite3_close(db_handler);
  /*free(timeslots);*/
  return 0;

}

struct Timeslot* read_entries(sqlite3 *handle, int *counter)
{
  sqlite3_stmt *stmt;
	
	sqlite3_prepare(handle, "SELECT * FROM timeslots", -1, &stmt, NULL);
	
  int step = sizeof(struct Timeslot);
  *counter = 0;
  struct Timeslot *timeslots = (struct Timeslot*) malloc(1 * step);
	while(sqlite3_step(stmt) != SQLITE_DONE)
	{
    if (*counter>0)
      timeslots = (struct Timeslot*) realloc(timeslots, (*counter+1) * step);
    int currentId = sqlite3_column_int(stmt, 0);
    const unsigned char *beginsRaw = sqlite3_column_text(stmt, 1);
    const unsigned char *endsRaw = sqlite3_column_text(stmt, 2);
    struct tm begins;
    struct tm ends; 
    if (strptime(beginsRaw, TIME_FORMAT , &begins) == 0)
      printf("Error while parsing \"begins\"\n");
    if (strptime(endsRaw, TIME_FORMAT, &ends) == 0)
      printf("Error while parsing \"ends\"\n");
    struct Timeslot timeslot = {
      .id = currentId,
      .begins = begins,
      .ends = ends
    };
    *(timeslots + (*counter*step)) = timeslot; 
    *counter+=1;
	}
  if (*counter==0)
  {
    free(timeslots);
    timeslots = NULL;
  }
	sqlite3_finalize(stmt);
  return timeslots;
}


void create_table(sqlite3 *handle)
{
	sqlite3_exec(handle, "CREATE TABLE timeslots (id integer primary key autoincrement, begins text, ends text);", NULL, NULL, NULL);
}


