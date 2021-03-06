#ifndef COMMON_H_OCCURED
#define COMMON_H_OCCURED

// needed for strptime on Linux systems
#define _XOPEN_SOURCE

#include <time.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "database.h"

extern const char * TIME_FORMAT;

#endif /* COMMON_H_OCCURED */
