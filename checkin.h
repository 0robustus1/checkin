#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
struct Timeslot
{
  int id;
  struct tm begins;
  struct tm ends;
};
struct Timeslot* read_entries(sqlite3 *handle, int *counter);
void create_timeslot(char *begins, char *ends, struct Timeslot *timeslot);
void create_table(sqlite3 *handle);
void show_timeslot(struct Timeslot *timeslot);
