#include "checkin.h"
#include <unistd.h>

const char * DATABASE_FILE = "times.db";
const char * CONFIG_PATH   = ".config/checkin";
const char * TIME_FORMAT   = "%Y-%m-%d %H:%M:00";
const int true = 1;
const int false = 0;

sqlite3 *db_handler;
int *db_open;
void initialize_database_connection();
void kill_database_connection();

int main(int argc, char *argv[])
{
  db_open = malloc( sizeof(int) );
  *db_open = false;

  if( argc == 1 )
    printf("no option or keyword specified...\n");
  else if( argc > 1)
  {
    char *keyword = NULL;
    int arg_c = argc;
    char **arg_v = argv;
    if( argv[1][0] != '-') /* not an option */
    {
      keyword = argv[1];
      arg_c -= 1;
      arg_v = argv+1;
    }
    initialize_database_connection();
    handle_options(keyword, arg_c, arg_v);
  }
  kill_database_connection();
  free(db_open);
  return 0;
}

void initialize_database_connection()
{
  char * db_file = (char *) malloc( 120 * sizeof(char) );
  sprintf(db_file, "%s/%s/%s",getenv("HOME"),CONFIG_PATH,DATABASE_FILE);
  sqlite3_open(db_file, &db_handler);
  *db_open = true;
  free(db_file);
}

void kill_database_connection()
{
  if( *db_open )
    sqlite3_close(db_handler);
}

void handle_options(char *keyword, int argc, char **argv)
{
  int current_opt;
  int mode = CheckinNoMode;
  int day, month, year, beginsHour, beginsMinute, endsHour, endsMinute;
  int dset = NoDateSet;
  int bset = false;
  int eset = false;
  int verbose = DontBeVerbose;
  while ((current_opt = getopt (argc, argv, "lsd:b:e:v")) != -1)
    switch(current_opt)
    {
      case 'l':
        mode = CheckinListing;
        break;
      case 's':
        mode = CheckinStatus;
        break;
      case 'd':
        if( sscanf(optarg, "%d.%d.%d", &day, &month, &year) != 3 )
        {
          if( sscanf(optarg, "%d/%d", &month, &year) != 2 ) 
          {
            if( verbose )
              puts("-d switch used in the wrong way...");
            exit(1);
          }
          else
            dset = DateWithoutDaySet;
        }
        else
          dset = DateSet;
        break;
      case 'b':
        sscanf(optarg, "%d:%d", &beginsHour, &beginsMinute);
        bset = true;
        break;
      case 'e':
        sscanf(optarg, "%d:%d", &endsHour, &endsMinute);
        eset = true;
        break;
      case 'v':
        verbose = BeVerbose;
        break;
      default:
        abort ();
    }
  time_t now_epoch;
  time(&now_epoch);
  struct tm *now = localtime(&now_epoch);
  if( mode != CheckinNoMode )
  {
    if( verbose && dset==DateSet )
        puts("Date set, ignoring day-value...");
    if( mode == CheckinListing )
      checkin_list(db_handler, now, (dset) ? &year : NULL, (dset) ? &month : NULL);
    else if( mode == CheckinStatus )
      checkin_status(db_handler, now, (dset) ? &year : NULL, (dset) ? &month : NULL);
    kill_database_connection();
    exit(0);
  } else
  {
    if( !(bset && eset) )
    {
      if( verbose )
        puts("Just one of {-b,-e} set, you need to set both...");
      exit(1);
    }
    if( dset==DateWithoutDaySet )
    {
      if( verbose )
        puts("You need to use the DD.MM.YYY format when adding...");
      exit(1);
    }
    if( !dset )
    {
      if( verbose )
        puts("No date set, using todays date...");
      year = now->tm_year+1900;
      month = now->tm_mon+1;
      day = now->tm_mday;
    }
    struct tm begins = {
      .tm_year  = year-1900,
      .tm_mon   = month-1,
      .tm_mday  = day,
      .tm_hour  = beginsHour-1,
      .tm_min   = beginsMinute
    };
    struct tm ends = {
      .tm_year  = year-1900,
      .tm_mon   = month-1,
      .tm_mday  = day,
      .tm_hour  = endsHour-1,
      .tm_min   = endsMinute
    };
    mktime(&begins);
    mktime(&ends);
    checkin_add(db_handler, &begins, &ends);
    exit(0);
  }
}

void checkin_add(sqlite3 *handle, struct tm *begins, struct tm *ends)
{
  char *request = (char *) malloc( 125 * sizeof(char) );
  char *beginsString = (char *) malloc( 20 * sizeof(char) );
  char *endsString = (char *) malloc( 20 * sizeof(char) );
  strftime(beginsString, 20, TIME_FORMAT, begins); 
  strftime(endsString, 20, TIME_FORMAT, ends); 
  sprintf(request,
          "INSERT INTO timeslots (begins,ends) VALUES (\"%s\",\"%s\");",
          beginsString,
          endsString
         );
  /*puts(request);*/
  sqlite3_stmt *stmt;
  sqlite3_prepare(handle, request, -1, &stmt, NULL);
  if( sqlite3_step(stmt) != SQLITE_DONE )
  {
    printf("Problem encountered while trying to save...\n");
    exit(1);
  }
  sqlite3_finalize(stmt);
}

void checkin_list(sqlite3 *handle, struct tm *now, int *overrideYear, int *overrideMonth)
{
  int entries = 0;
  struct Timeslot *timeslots;
  char *request = (char *) malloc( 81 * sizeof(char) );
  struct tm *usedTime;
  if( overrideYear && overrideMonth )
  {
    struct tm tmp = {
      .tm_year = *overrideYear-1900,
      .tm_mon  = *overrideMonth-1
    };
    strftime(request, 81, "SELECT * FROM timeslots WHERE ends LIKE \"%Y-%m%%\";", &tmp);
    usedTime = &tmp;
  } else
  {
    strftime(request, 81, "SELECT * FROM timeslots WHERE ends LIKE \"%Y-%m%%\";", now);
    usedTime = now;
  }
  timeslots = read_entries(handle, &entries, request);
  if (timeslots == NULL) 
  {
    /*puts(request);*/
    /*puts("Problem while parsing...");*/
    /*exit(1);*/
  }
  free(request);
  print_month(timeslots, entries, usedTime->tm_year, usedTime->tm_mon);
  free(timeslots);
}

void checkin_status(sqlite3 *handle, struct tm *now, int *overrideYear, int *overrideMonth)
{
  int entries = 0;
  int i;
  struct Timeslot *timeslots;
  char *request = (char *) malloc( 81 * sizeof(char) );
  struct tm *usedTime;
  if( overrideYear && overrideMonth )
  {
    struct tm tmp = {
      .tm_year = *overrideYear-1900,
      .tm_mon  = *overrideMonth-1
    };
    strftime(request, 81, "SELECT * FROM timeslots WHERE ends LIKE \"%Y-%m%%\";", &tmp);
    usedTime = &tmp;
  } else
  {
    strftime(request, 81, "SELECT * FROM timeslots WHERE ends LIKE \"%Y-%m%%\";", now);
    usedTime = now;
  }
  timeslots = read_entries(handle, &entries, request);
  char *output = request;
  strftime(output, 81, "Status for month: %m/%Y\n\n",usedTime);
  printf("%s",output);
  int time_sum = 0;
  int hours = 0;
  int minutes = 0;
  for(i=0;i<entries;i++)
  {
    hours = 0;
    minutes = 0;
    calculate_difference( (timeslots+i), &hours, &minutes );
    time_sum += hours*60 + minutes;
  }
  printf("Overall working time: %.3dh %.2dm\n", time_sum/60, time_sum%60);

  free(output);
  free(timeslots);
}

struct Timeslot* read_entries(sqlite3 *handle, int *counter, char *request)
{
  sqlite3_stmt *stmt;

  sqlite3_prepare(handle, request, -1, &stmt, NULL);

  int step = sizeof(struct Timeslot);
  *counter = 0;
  struct Timeslot *timeslots = (struct Timeslot*) malloc(1 * step);
  while(sqlite3_step(stmt) != SQLITE_DONE)
  {
    if (*counter>0)
      timeslots = (struct Timeslot*) realloc(timeslots, (*counter+1) * step);
    int currentId = sqlite3_column_int(stmt, 0);
    const char *beginsRaw = (const char *) sqlite3_column_text(stmt, 1);
    const char *endsRaw = (const char *) sqlite3_column_text(stmt, 2);
    struct tm begins;
    struct tm ends; 
    if (strptime(beginsRaw, TIME_FORMAT , &begins) == 0)
      printf("Error while parsing \"begins\"\n");
    if (strptime(endsRaw, TIME_FORMAT, &ends) == 0)
      printf("Error while parsing \"ends\"\n");
    /*Wrong values in tm_sec could alter important values*/
    begins.tm_sec = 0;
    begins.tm_sec = 0;
    /*Normalize the values.*/
    /*mktime(&begins);*/
    /*mktime(&ends);*/
    struct Timeslot timeslot = {
      .id = currentId,
      .begins = begins,
      .ends = ends
    };
    *(timeslots + (*counter)) = timeslot; 
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


