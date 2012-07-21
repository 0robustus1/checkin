#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "timeslot.h"

struct Timeslot* read_entries(sqlite3 *handle, int *counter, char *request);
void create_table(sqlite3 *handle);
void checkin_list();
void checkin_status();
