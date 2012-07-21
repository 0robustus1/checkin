#ifndef TIMESLOT_H_OCCURED
#define TIMESLOT_H_OCCURED

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Timeslot
{
  int id;
  struct tm begins;
  struct tm ends;
};
//void create_timeslot(char *begins, char *ends, struct Timeslot *timeslot);
void show_timeslot(struct Timeslot *ts);
void calculate_difference(struct Timeslot *ts, int *hours, int *minutes);
void print_month(struct Timeslot *timeslots, int ts_count, int year, int month);

#endif /* TIMESLOT_H_OCCURED */
