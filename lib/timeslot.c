#include "timeslot.h"

void show_timeslot(struct Timeslot *ts)
{
  printf("Timeslot (%i)\n",ts->id);
  int hours = 0;
  int minutes = 0;
  calculate_difference(ts,&hours,&minutes);
  if( ((ts->begins).tm_mday == (ts->ends).tm_mday) && ((ts->begins).tm_mon == (ts->ends).tm_mon) && ((ts->begins).tm_year == (ts->ends).tm_year) )
  {
    char *dateOutput = (char *) malloc(11 * sizeof(char));
    char *beginsOutput = (char *) malloc( 6 * sizeof(char));
    char *endsOutput = (char *) malloc( 6 * sizeof(char));
    strftime(dateOutput, 11, "%d.%m.%Y",&(ts->begins));
    strftime(beginsOutput, 6, "%H:%M", &(ts->begins));
    strftime(endsOutput, 6, "%H:%M", &(ts->ends));
    printf("\t%s\n",dateOutput);
    printf("\tStarted: %s\n",beginsOutput);
    printf("\tEnded:   %s\n",endsOutput);
  } 
  else
  {
    char *beginsOutput = (char *) malloc( 20 * sizeof(char));
    char *endsOutput = (char *) malloc( 20 * sizeof(char));
    strftime(beginsOutput, 20, "%d.%m.%Y %T", &(ts->begins));
    strftime(endsOutput, 20, "%d.%m.%Y %T", &(ts->ends));
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
    {
      *hours -= 1;
      *minutes = 60 - *minutes;
    }

  }
}


void print_month(struct Timeslot *timeslots, int ts_count, int year, int month)
{
  struct tm run_day = {.tm_year = year, .tm_mon = month, .tm_mday = 1};
  struct tm last_day = {.tm_year = year, .tm_mon = month+1, .tm_mday = 0};
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
    struct Timeslot currentSlot = *(timeslots + i);
    calculate_difference( &currentSlot, &hours, &minutes );
    *(worked_days_in_minutes + (currentSlot.begins).tm_mday-1) += minutes + hours*60;
    
  }
  int weeks_time = 0;
  int total = 0;
  for(i=0;i<days_in_month;i++) 
  {
    char *prefix = malloc( 15 * sizeof(char));
    strftime(prefix, 15, "%a, %d.%m ## ", &run_day);
    printf("%s\t%.2dh %.2dm ###\n",prefix,*(worked_days_in_minutes + i)/60, *(worked_days_in_minutes + i)%60);
    weeks_time += *(worked_days_in_minutes + i);
    if( run_day.tm_wday == 0 ) 
    {
      printf("###########################\n");
      printf("############### %.2dh %.2dm ###\n", weeks_time/60, weeks_time%60);
      printf("###########################\n");
      total += weeks_time;
      weeks_time = 0;
    }
    run_day.tm_mday += 1;
    mktime(&run_day);
  }
  printf("###########################\n");
  printf("####### Total: %.3dh %.2dm ###\n", total/60, total%60);
  printf("###########################\n");
}
