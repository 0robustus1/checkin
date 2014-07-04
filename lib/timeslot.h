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
struct Timeslot* timeslot_create(int id, const char *begins_raw, const char *ends_raw, struct Timeslot* slot);
struct tm* tm_create_from_raw(const char *raw);
void tm_destroy(struct tm *tm_p);
void timeslot_destroy(struct Timeslot* timeslot_p);

#endif /* TIMESLOT_H_OCCURED */
