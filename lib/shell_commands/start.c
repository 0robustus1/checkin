#include "shell_commands.h"

bool handle_start()
{
  printf("encountered %s.\n", "start");
  if( !current_start ) {
    current_start = now();
    printf("Started Session at: ");
    print_time(current_start, "\n");
  } else {
    report_status();
  }
  return true;
}
