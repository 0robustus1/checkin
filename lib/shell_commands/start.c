#include "shell_commands.h"

bool handle_start()
{
  printf("encountered %s.\n", "start");
  if( !current_start ) {
    current_start = now();
    printf("Started Session at: ");
    print_time(current_start, "\n");
  } else {
    printf("Already running a session, since: ");
    print_time(current_start, "\n");
  }
  return true;
}
