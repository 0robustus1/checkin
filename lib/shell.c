#include "shell.h"

static const int max_line_length = 160;
const char *in_line_delimiters = " \n";
static const char *prompt = "±> ";

static char *line;

static void setup_shell();
static void teardown_shell();

static bool process_line();

static shell_tuple_t shell_commands[] = {
  {.keyword = "start", .shell_command = handle_start},
  {.keyword = "stop", .shell_command = handle_stop},
  {.keyword = "list", .shell_command = handle_list},
  {.keyword = "exit", .shell_command = handle_exit},
  {.keyword = "report", .shell_command = handle_report},

  // end of array effectively represents NULL value
  {.keyword = NULL, .shell_command = NULL},
};

void checkin_shell()
{
  setup_shell();

  while(true) {
    printf("%s", prompt);
    fflush(NULL);
    if( fgets(line, max_line_length, stdin) ) {
      if( !process_line() ) break;
    } else {
      break;
    }
  }

  teardown_shell();
}

/*
 * Returns "boolean" which represents whether the program
 * should continue.
 */
bool process_line()
{
  char *cmd = strtok(line, in_line_delimiters);

  for(int index = 0; shell_commands[index].keyword; index++) {
    if( !strcmp(cmd, shell_commands[index].keyword) )
      return shell_commands[index].shell_command();
  }

  fflush(NULL);
  return true;
}

void setup_shell()
{
  line = (char *) malloc(max_line_length * sizeof(char));
  initialize_database_connection();
}

void teardown_shell()
{
  free(line);
  kill_database_connection();
}
