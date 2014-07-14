#include "common.h"
#include "timeslot.h"

static const char *table_name = "timeslots";

static bool timeslot_persist_with_db(const timeslot_p timeslot, sqlite3 *handle);
static timeslot_p timeslot_save_with_db(const tm_p begins_day, int begins_hour, int begins_minute, const tm_p ends_day, int ends_hour, int ends_minute, sqlite3 *handle);

timeslot_p timeslot_create(int id, const char *begins_raw, const char *ends_raw, const timeslot_p slot)
{
  timeslot_p timeslot = slot;
  tm_p begins = tm_create_from_raw(begins_raw);
  tm_p ends = tm_create_from_raw(ends_raw);
  bool malloced = false;

  if( !slot ) {
    timeslot = malloc(sizeof(timeslot_t));
    malloced = true;
  }

  if( !(begins && ends) ) {
    if( malloced )
      timeslot_destroy(timeslot);
    return NULL;
  }

  timeslot->id = id;
  timeslot->begins = *begins;
  timeslot->ends = *ends;

  tm_destroy(begins);
  tm_destroy(ends);

  return timeslot;
}

tm_p now()
{
  time_t now_epoch;
  time(&now_epoch);
  tm_p now = localtime(&now_epoch);
  tm_p tm = malloc(sizeof(tm_t));
  *tm = *now;
  return tm;
}

timeslot_p timeslot_save(const tm_p begins_day, int begins_hour, int begins_minute, const tm_p ends_day, int ends_hour, int ends_minute)
{
  return timeslot_save_with_db(begins_day, begins_hour, begins_minute,
      ends_day, ends_hour, ends_minute, db_handler);
}

static timeslot_p timeslot_save_with_db(const tm_p begins_day, int begins_hour, int begins_minute, const tm_p ends_day, int ends_hour, int ends_minute, sqlite3 *handle)
{
  tm_p today_p = today();
  timeslot_p timeslot = malloc( sizeof(timeslot_t) );

  timeslot->begins = begins_day ? *begins_day : *today_p;
  timeslot->begins.tm_hour = begins_hour - 1;
  timeslot->begins.tm_min = begins_minute;
  timeslot->ends = ends_day ? *ends_day : *today_p;
  timeslot->ends.tm_hour = ends_hour - 1;
  timeslot->ends.tm_min = ends_minute;

  timeslot_persist_with_db(timeslot, handle);
  tm_destroy(today_p);
  return timeslot;
}

bool timeslot_persist(const timeslot_p timeslot)
{
  return timeslot_persist_with_db(timeslot, db_handler);
}

static bool timeslot_persist_with_db(const timeslot_p timeslot, sqlite3 *handle)
{
  char *request = malloc( 125 * sizeof(char) );
  char *beginsString = malloc( 20 * sizeof(char) );
  char *endsString = malloc( 20 * sizeof(char) );
  strftime(beginsString, 20, TIME_FORMAT, &timeslot->begins);
  strftime(endsString, 20, TIME_FORMAT, &timeslot->ends);
  sprintf(request,
          "INSERT INTO %s (begins,ends) VALUES (\"%s\",\"%s\");",
          table_name, beginsString, endsString);
  sqlite3_stmt *stmt;
  sqlite3_prepare(handle, request, -1, &stmt, NULL);
  if( sqlite3_step(stmt) != SQLITE_DONE ) {
    printf("Problem encountered while trying to save...\n");
    return false;
  }
  sqlite3_finalize(stmt);
  timeslot->id = retrieve_latest_id_for(handle, table_name);
  return true;
}

void timeslot_round(timeslot_p timeslot, int margin_minutes)
{
  timeslot->begins = tm_round(timeslot->begins, margin_minutes);
  timeslot->ends = tm_round(timeslot->begins, margin_minutes);
}

tm_t tm_round(tm_t tm, int margin_minutes)
{
  int margin = margin_minutes == 0 ? 60 : margin_minutes;
  int multiplicator = tm.tm_min / margin;
  int offset = tm.tm_min % margin;
  if( offset > (margin / 2) )
    multiplicator++;
  tm.tm_min =  multiplicator * margin;
  mktime(&tm);
  return tm;
}

tm_p tm_create_from_raw(const char *raw)
{
  tm_p tm = malloc(sizeof(tm_t));
  if (strptime(raw, TIME_FORMAT , tm) != 0) {
    /*Wrong values in tm_sec could alter important values*/
    tm->tm_sec = 0;
    tm->tm_sec = 0;
    return tm;
  } else {
    tm_destroy(tm);
    return NULL;
  }
}

void tm_destroy(tm_p tm)
{
  free(tm);
}

void timeslot_destroy(const timeslot_p timeslot)
{
  free(timeslot);
}

/*
 * Returns boolean for the question if begins and ends
 * of a Timeslot refer to the same exact day.
 */
int timeslot_same_day(timeslot_p ts)
{
  return ((ts->begins).tm_mday == (ts->ends).tm_mday) && // Day of Month is equal
    ((ts->begins).tm_mon == (ts->ends).tm_mon) && // Month is equal
    ((ts->begins).tm_year == (ts->ends).tm_year); // Year is equal
}

void show_timeslot(timeslot_p ts)
{
  printf("Timeslot (%i)\n",ts->id);
  int hours = 0;
  int minutes = 0;
  calculate_difference(ts,&hours,&minutes);
  char *beginsOutput, *endsOutput;
  if( timeslot_same_day(ts) ) {
    char *dateOutput = (char *) malloc(11 * sizeof(char));
    beginsOutput = (char *) malloc( 6 * sizeof(char));
    endsOutput = (char *) malloc( 6 * sizeof(char));
    strftime(dateOutput, 11, "%d.%m.%Y",&(ts->begins));
    strftime(beginsOutput, 6, "%H:%M", &(ts->begins));
    strftime(endsOutput, 6, "%H:%M", &(ts->ends));
    printf("\t%s\n",dateOutput);
    printf("\tStarted: %s\n",beginsOutput);
    printf("\tEnded:   %s\n",endsOutput);
    free(dateOutput);
  } else {
    beginsOutput = (char *) malloc( 20 * sizeof(char));
    endsOutput = (char *) malloc( 20 * sizeof(char));
    strftime(beginsOutput, 20, "%d.%m.%Y %T", &(ts->begins));
    strftime(endsOutput, 20, "%d.%m.%Y %T", &(ts->ends));
    printf("\tStarted: %s\n",beginsOutput);
    printf("\tEnded:   %s\n",endsOutput);
  }
  free(beginsOutput);
  free(endsOutput);
  printf("\tDuration: ");
  if( hours!=0 )
    printf("%ih ",hours);
  printf("%im",minutes);
  printf("\n");
}


void calculate_difference(timeslot_p ts, int *hours, int *minutes) {
  if( timeslot_same_day(ts) ) {
    *hours = abs( (ts->ends).tm_hour - (ts->begins).tm_hour );
    *minutes = abs( (ts->ends).tm_min - (ts->begins).tm_min );
    if( (ts->ends).tm_min < (ts->begins).tm_min ) {
      *hours -= 1;
      *minutes = 60 - *minutes;
    }

  }
}


void print_month(timeslot_p timeslots, int ts_count, int year, int month)
{
  tm_t run_day = {.tm_year = year, .tm_mon = month, .tm_mday = 1};
  tm_t last_day = {.tm_year = year, .tm_mon = month+1, .tm_mday = 0};
  mktime(&run_day);
  mktime(&last_day);
  int days_in_month = last_day.tm_mday;
  int * worked_days_in_minutes = (int *) malloc(days_in_month * sizeof(int));
  int i;
  int hours = 0;
  int minutes = 0;
  for(i=0;i<days_in_month;i++)
    *(worked_days_in_minutes + i) = 0;
  for(i=0;i<ts_count;i++) {
    hours = 0;
    minutes = 0;
    timeslot_t currentSlot = *(timeslots + i);
    calculate_difference( &currentSlot, &hours, &minutes );
    *(worked_days_in_minutes + (currentSlot.begins).tm_mday-1) += minutes + hours*60;
  }
  int weeks_time = 0;
  int total = 0;
  for(i=0;i<days_in_month;i++) {
    char *prefix = malloc( 15 * sizeof(char));
    strftime(prefix, 15, "%a, %d.%m ## ", &run_day);
    printf("%s\t%.2dh %.2dm ###\n",prefix, *(worked_days_in_minutes + i)/60, *(worked_days_in_minutes + i)%60);
    free(prefix);
    weeks_time += *(worked_days_in_minutes + i);
    if( run_day.tm_wday == 0 || i==days_in_month-1 ) {
      printf("###########################\n");
      printf("############### %.2dh %.2dm ###\n", weeks_time/60, weeks_time%60);
      printf("###########################\n");
      total += weeks_time;
      weeks_time = 0;
    }
    run_day.tm_mday += 1;
    mktime(&run_day);
  }
  puts("");
  total += weeks_time;
  printf("###########################\n");
  printf("####### Total: %.3dh %.2dm ###\n", total/60, total%60);
  printf("###########################\n");
  free(worked_days_in_minutes);
}
