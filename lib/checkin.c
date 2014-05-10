#include "checkin.h"

const int true = 1;
const int false = 0;

void checkin_initialize();
void checkin_terminate(int exit_state);

const char * TIME_FORMAT   = "%Y-%m-%d %H:%M:00";
const char * FETCH_MONTH_QUERY = "SELECT id,begins,ends "
                                 "FROM timeslots "
                                 "WHERE ends LIKE \"%Y-%m%%\";";

int *mode, *verbose, *b_opt_set, *d_opt_set, *e_opt_set;


int main(int argc, char *argv[])
{
  checkin_initialize();
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
  checkin_terminate(0);
}

void checkin_initialize()
{
  db_open = malloc( sizeof(int) );
  *db_open = false;

  mode = malloc( sizeof(int) );
  *mode = CheckinNoMode;

  verbose = malloc( sizeof(int) );
  *verbose = DontBeVerbose;


  b_opt_set = malloc( sizeof(int) );
  *b_opt_set = false;

  d_opt_set = malloc( sizeof(int) );
  *d_opt_set = NoDateSet;

  e_opt_set = malloc( sizeof(int) );
  *e_opt_set = false;
}

void checkin_terminate(int exit_state)
{
  kill_database_connection();

  free(db_open);
  free(mode);
  free(verbose);
  free(b_opt_set);
  free(d_opt_set);
  free(e_opt_set);

  exit(exit_state);
}

void out(char *output)
{
  if( *verbose )
    puts(output);
}

void handle_options(char *keyword, int argc, char **argv)
{
  int current_opt;
  int day, month, year, beginsHour, beginsMinute, endsHour, endsMinute;
  while ((current_opt = getopt (argc, argv, "lsd:b:e:v")) != -1)
    switch(current_opt)
    {
      case 'l':
        *mode = CheckinListing;
        break;
      case 's':
        *mode = CheckinStatus;
        break;
      case 'd':
        if( sscanf(optarg, "%d.%d.%d", &day, &month, &year) != 3 )
        {
          if( sscanf(optarg, "%d/%d", &month, &year) != 2 ) 
          {
            out("-d switch used in the wrong way...");
            checkin_terminate(1);
          }
          else
            *d_opt_set = DateWithoutDaySet;
        }
        else
          *d_opt_set = DateSet;
        break;
      case 'b':
        sscanf(optarg, "%d:%d", &beginsHour, &beginsMinute);
        *b_opt_set = true;
        break;
      case 'e':
        sscanf(optarg, "%d:%d", &endsHour, &endsMinute);
        *e_opt_set = true;
        break;
      case 'v':
        *verbose = BeVerbose;
        break;
      default:
        abort ();
    }
  time_t now_epoch;
  time(&now_epoch);
  struct tm *now = localtime(&now_epoch);
  if( *mode != CheckinNoMode )
  {
    if( *d_opt_set==DateSet )
      out("Date set, ignoring day-value...");
    if( *mode == CheckinListing )
      checkin_list(db_handler, now, (*d_opt_set) ? &year : NULL, (*d_opt_set) ? &month : NULL);
    else if( *mode == CheckinStatus )
      checkin_status(db_handler, now, (*d_opt_set) ? &year : NULL, (*d_opt_set) ? &month : NULL);
    kill_database_connection();
    checkin_terminate(0);
  } else
  {
    if( !(*b_opt_set && *e_opt_set) )
    {
      out("Just one of {-b,-e} set, you need to set both...");
      checkin_terminate(1);
    }
    if( *d_opt_set==DateWithoutDaySet )
    {
      out("You need to use the DD.MM.YYY format when adding...");
      checkin_terminate(1);
    }
    if( !*d_opt_set )
    {
      out("No date set, using todays date...");
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
    checkin_terminate(0);
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
    checkin_terminate(1);
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
    strftime(request, 81, FETCH_MONTH_QUERY, &tmp);
    usedTime = &tmp;
  } else
  {
    strftime(request, 81, FETCH_MONTH_QUERY, now);
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
    strftime(request, 81, FETCH_MONTH_QUERY, &tmp);
    usedTime = &tmp;
  } else
  {
    strftime(request, 81, FETCH_MONTH_QUERY, now);
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
