#ifndef COMMANDS_H_OCCURED
#define COMMANDS_H_OCCURED

#include "common.h"
#include "timeslot.h"
#include "database.h"

timeslot_p read_entries(sqlite3 *handle, int *counter, char *request);
void checkin_list(sqlite3 *handle, tm_p now, int *overrideYear, int *overrideMonth);
void checkin_status(sqlite3 *handle, tm_p now, int *overrideYear, int *overrideMonth);
int checkin_add(sqlite3 *handle, tm_p begins, tm_p ends);

#endif /* COMMANDS_H_OCCURED */
