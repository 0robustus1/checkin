#ifndef TIMESLOT_H_OCCURED
#define TIMESLOT_H_OCCURED

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define today() now()

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
void print_month(timeslot_p timeslots, int ts_count, int year, int month);
timeslot_p timeslot_create(int id, const char *begins_raw, const char *ends_raw, timeslot_p slot);
tm_p tm_create_from_raw(const char *raw);
timeslot_p timeslot_save(const tm_p begins_day, int begins_hour, int begins_minute, const tm_p ends_day, int ends_hour, int ends_minute);
bool timeslot_persist(const timeslot_p timeslot);
void timeslot_round(timeslot_p timeslot, int margin_minutes);
int timeslot_length(timeslot_p timeslot);
void timeslot_inner_difference(timeslot_p timeslot, int *hours, int *minutes);
tm_t tm_round(tm_t tm, int margin_minutes);
void tm_destroy(tm_p tm);
void timeslot_destroy(timeslot_p timeslot);
tm_p now();

#endif /* TIMESLOT_H_OCCURED */
