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

typedef struct Timeslot timeslot_t;
typedef timeslot_t* timeslot_p;

typedef struct tm tm_t;
typedef tm_t* tm_p;

void show_timeslot(timeslot_p ts);
void calculate_difference(timeslot_p ts, int *hours, int *minutes);
void print_month(timeslot_p timeslots, int ts_count, int year, int month);
timeslot_p timeslot_create(int id, const char *begins_raw, const char *ends_raw, timeslot_p slot);
tm_p tm_create_from_raw(const char *raw);
void tm_destroy(tm_p tm);
void timeslot_destroy(timeslot_p timeslot);

#endif /* TIMESLOT_H_OCCURED */
