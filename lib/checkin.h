#include "common.h"
#include "timeslot.h"
#include "database.h"
#include "shell.h"
#include "commands.h"

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
