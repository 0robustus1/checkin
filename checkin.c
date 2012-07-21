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
  /*printf("Counter is %i and Starttime is: ",entries);*/
  /*printf(asctime(&(timeslots->begins)));*/
  /*printf("\n");*/
  /*printf("id is: %i\n",(timeslots->id));*/
  int i;
  for(i=0;i<entries;i++)
    show_timeslot(timeslots + (i*sizeof(struct Timeslot)));
  sqlite3_close(db_handler);
  free(timeslots);
  return 0;

}

struct Timeslot* read_entries(sqlite3 *handle, int *counter)
{
  sqlite3_stmt *stmt;
	
	sqlite3_prepare(handle, "SELECT * FROM timeslots", -1, &stmt, NULL);
	
  /*printf("ID\tName\t\tVorname\n");*/
  int step = sizeof(struct Timeslot);
  *counter = 0;
  struct Timeslot *timeslots = (struct Timeslot*) malloc(1 * step);
	while(sqlite3_step(stmt) != SQLITE_DONE)
	{
    /*printf("Counter is now: %i\n",*counter);*/
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
		/* Ausgabe der einzelnen Datenfeldern als int und text */
		/*printf("%i\t%s\t\t%s\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2));*/
	}
  if (*counter==0)
  {
    free(timeslots);
    timeslots = NULL;
  }
	/* Loeschen des Statements */
	sqlite3_finalize(stmt);
  return timeslots;
}


void create_table(sqlite3 *handle)
{
	sqlite3_exec(handle, "CREATE TABLE timeslots (id integer primary key autoincrement, begins text, ends text);", NULL, NULL, NULL);
}

void show_timeslot(struct Timeslot *timeslot)
{
  printf("Timeslot (%i)\n",timeslot->id);
  int hours = 0;
  int minutes = 0;
  calculate_difference(timeslot,&hours,&minutes);
  if( ((timeslot->begins).tm_mday == (timeslot->ends).tm_mday) && ((timeslot->begins).tm_mon == (timeslot->ends).tm_mon) && ((timeslot->begins).tm_year == (timeslot->ends).tm_year) )
  {
    char *dateOutput = (char *) malloc(11 * sizeof(char));
    char *beginsOutput = (char *) malloc( 9 * sizeof(char));
    char *endsOutput = (char *) malloc( 9 * sizeof(char));
    strftime(dateOutput, 11, "%d.%m.%Y",&(timeslot->begins));
    strftime(beginsOutput, 9, "%T", &(timeslot->begins));
    strftime(endsOutput, 9, "%T", &(timeslot->ends));
    printf("\t%s\n",dateOutput);
    printf("\tStarted: %s\n",beginsOutput);
    printf("\tEnded:   %s\n",endsOutput);
  } 
  else
  {
    char *beginsOutput = (char *) malloc( 20 * sizeof(char));
    char *endsOutput = (char *) malloc( 20 * sizeof(char));
    strftime(beginsOutput, 20, "%d.%m.%Y %T", &(timeslot->begins));
    strftime(endsOutput, 20, "%d.%m.%Y %T", &(timeslot->ends));
    printf("\tStarted: %s\n",beginsOutput);
    printf("\tEnded:   %s\n",endsOutput);
  }
  printf("\tDuration: ");
  if( hours!=0 )
    printf("%ih ",hours);
  printf("%im",minutes);
  printf("\n");
}


void calculate_difference(struct Timeslot *ts, int *hours, int *minutes)
{
  if( ((ts->begins).tm_mday == (ts->ends).tm_mday) && ((ts->begins).tm_mon == (ts->ends).tm_mon) && ((ts->begins).tm_year == (ts->ends).tm_year) )
  {
    *hours = abs( (ts->ends).tm_hour - (ts->begins).tm_hour );
    *minutes = abs( (ts->ends).tm_min - (ts->begins).tm_min );
    if( (ts->ends).tm_min < (ts->begins).tm_min )
      *hours -= 1;
  }
}
