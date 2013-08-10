#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "timeslot.h"

struct Timeslot* read_entries(sqlite3 *handle, int *counter, char *request);
void create_table(sqlite3 *handle);
void checkin_list(sqlite3 *handle, struct tm *now, int *overrideYear, int *overrideMonth);
void checkin_status(sqlite3 *handle, struct tm *now, int *overrideYear, int *overrideMonth);
void checkin_add(sqlite3 *handle, struct tm *begins, struct tm *ends);
void out(char *output);
void handle_options(char *keyword, int argc, char **argv);

enum CheckinModes {
  CheckinNoMode, CheckinListing, CheckinStatus
};
enum CheckinDateModes {
  NoDateSet, DateSet, DateWithoutDaySet
};
enum CheckinVerbosity {
  DontBeVerbose, BeVerbose
};
