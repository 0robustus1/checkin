#ifndef COMMANDS_H_OCCURED
#define COMMANDS_H_OCCURED

#include "common.h"
#include "timeslot.h"
#include "database.h"

struct Timeslot* read_entries(sqlite3 *handle, int *counter, char *request);
void checkin_list(sqlite3 *handle, struct tm *now, int *overrideYear, int *overrideMonth);
void checkin_status(sqlite3 *handle, struct tm *now, int *overrideYear, int *overrideMonth);
int checkin_add(sqlite3 *handle, struct tm *begins, struct tm *ends);

#endif /* COMMANDS_H_OCCURED */
