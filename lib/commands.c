#include "commands.h"

const char * FETCH_MONTH_QUERY = "SELECT id,begins,ends "
                                 "FROM timeslots "
                                 "WHERE ends LIKE \"%Y-%m%%\";";
/*
 * Returns "boolean" which represents whether the execution
 * was successful.
 */
int checkin_add(sqlite3 *handle, tm_p begins, tm_p ends)
{
  char *request = (char *) malloc( 125 * sizeof(char) );
  char *beginsString = (char *) malloc( 20 * sizeof(char) );
  char *endsString = (char *) malloc( 20 * sizeof(char) );
  strftime(beginsString, 20, TIME_FORMAT, begins);
  strftime(endsString, 20, TIME_FORMAT, ends);
  sprintf(request,
          "INSERT INTO timeslots (begins,ends) VALUES (\"%s\",\"%s\");",
          beginsString,
          endsString);
  sqlite3_stmt *stmt;
  sqlite3_prepare(handle, request, -1, &stmt, NULL);
  if( sqlite3_step(stmt) != SQLITE_DONE ) {
    printf("Problem encountered while trying to save...\n");
    return false;
  }
  sqlite3_finalize(stmt);
  return true;
}

void checkin_list(sqlite3 *handle, tm_p now, int *overrideYear, int *overrideMonth)
{
  int entries = 0;
  timeslot_p timeslots;
  char *request = (char *) malloc( 81 * sizeof(char) );
  tm_p usedTime;
  if( overrideYear || overrideMonth ) {
    tm_t tmp = {
      .tm_year = overrideYear ? *overrideYear-1900 : now->tm_year,
      .tm_mon  = overrideMonth ? *overrideMonth-1 : now->tm_mon
    };
    strftime(request, 81, FETCH_MONTH_QUERY, &tmp);
    usedTime = &tmp;
  } else {
    strftime(request, 81, FETCH_MONTH_QUERY, now);
    usedTime = now;
  }
  timeslots = read_entries(handle, &entries, request);
  if (timeslots == NULL) {
    /*puts("Problem while parsing...");*/
    /*exit(1);*/
  }
  free(request);
  print_month(timeslots, entries, usedTime->tm_year, usedTime->tm_mon);
  free(timeslots);
}

void checkin_status(sqlite3 *handle, tm_p now, int *overrideYear, int *overrideMonth)
{
  int entries = 0;
  int i;
  timeslot_p timeslots;
  char *request = (char *) malloc( 81 * sizeof(char) );
  tm_p usedTime;
  if( overrideYear && overrideMonth ) {
    tm_t tmp = {
      .tm_year = *overrideYear-1900,
      .tm_mon  = *overrideMonth-1
    };
    strftime(request, 81, FETCH_MONTH_QUERY, &tmp);
    usedTime = &tmp;
  } else {
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
  for(i=0;i<entries;i++) {
    hours = 0;
    minutes = 0;
    timeslot_inner_difference( (timeslots+i), &hours, &minutes );
    time_sum += hours*60 + minutes;
  }
  printf("Overall working time: %.3dh %.2dm\n", time_sum/60, time_sum%60);

  free(output);
  free(timeslots);
}

timeslot_p read_entries(sqlite3 *handle, int *counter, char *request)
{
  sqlite3_stmt *stmt;

  sqlite3_prepare(handle, request, -1, &stmt, NULL);

  int step = sizeof(timeslot_t);
  *counter = 0;
  timeslot_p timeslots = malloc(1 * step);
  while(sqlite3_step(stmt) != SQLITE_DONE) {
    if (*counter>0)
      timeslots = realloc(timeslots, (*counter+1) * step);
    int currentId = sqlite3_column_int(stmt, 0);
    const char *beginsRaw = (const char *) sqlite3_column_text(stmt, 1);
    const char *endsRaw = (const char *) sqlite3_column_text(stmt, 2);
    timeslot_p timeslot = timeslot_create(currentId, beginsRaw, endsRaw, (timeslots + *counter));
    if( timeslot ) {
      *counter+=1;
    } else {
      fprintf(stderr, "Could not retrieve timeslot from database.\n");
    }
  }
  if (*counter==0) {
    free(timeslots);
    timeslots = NULL;
  }
  sqlite3_finalize(stmt);
  return timeslots;
}
