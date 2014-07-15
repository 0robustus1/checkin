#include "shell_commands.h"

bool handle_stop()
{
  printf("encountered %s.\n", "stop");
  if( current_start ) {
    tm_p current_stop = now();
    *current_start = tm_round(*current_start, margin_minutes);
    *current_stop = tm_round(*current_stop, margin_minutes);
    printf("Stopped...\n");
    printf("Session-Info:\n");
    printf("Start: ");
    print_time(current_start, "\n");
    printf("End: ");
    print_time(current_stop, "\n");

    int store = ask_for_confirmation("Store the timeslot?");

    if( store ) {
      printf("Ok, i will store the timeslot...\n");
      checkin_add(db_handler, current_start, current_stop);
    } else {
      printf("Storing of timeslot aborted.\n");
    }

    tm_destroy(current_start);
    current_start = NULL;
    tm_destroy(current_stop);
  } else {
    printf("There is no current session.\n");
  }
  return true;
}
