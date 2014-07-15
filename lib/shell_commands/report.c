#include "shell_commands.h"

bool handle_report()
{
  tm_p current = now();
  checkin_status(db_handler, current, NULL, NULL);
  printf("####################\n");
  report_status();
  return true;
}

void report_status()
{
  if( current_start ) {
    tm_p current = now();
    printf("running a session since: ");
    print_time(current_start, "\n");
    print_duration(current_start, current);
    tm_destroy(current);
  }
}

void print_duration(tm_p start, tm_p end)
{
  timeslot_p timeslot = timeslot_create_from_members(NULL, start, end, NULL);
  int length = timeslot_length(timeslot);
  char *multiple = length == 1 ? "" : "s";
  printf("current duration is: %i minute%s\n", length, multiple);
}
