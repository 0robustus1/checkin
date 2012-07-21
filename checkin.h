#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "timeslot.h"

struct Timeslot* read_entries(sqlite3 *handle, int *counter);
void create_table(sqlite3 *handle);

