#include "shell.h"

static const int max_line_length = 160;
const char *in_line_delimiters = " \n";
static const char *prompt = "±> ";

static char *line;

static void setup_shell();
static void teardown_shell();

static bool process_line();

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
int process_line()
{
  char *cmd = strtok(line, in_line_delimiters);

  if( !strcmp(cmd, "exit") ) {
    return false;
  } else if( !strcmp(cmd, "start") ) {
    return handle_start();
  } else if( !strcmp(cmd, "stop") ) {
    return handle_stop();
  } else if( !strcmp(cmd, "list") ) {
    return handle_list();
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
