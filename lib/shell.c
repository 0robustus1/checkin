#include "shell.h"

const int max_line_length = 160;
const char *in_line_delimiters = " \n";
const char *prompt = "±> ";

char *line;

void setup_shell();
void teardown_shell();

int process_line();

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
    printf("encountered %s.\n", cmd);
  } else if( !strcmp(cmd, "stop") ) {
    printf("encountered %s.\n", cmd);
  }

  fflush(NULL);
  return true;
}

void setup_shell()
{
  line = (char *) malloc(max_line_length * sizeof(char));
}

void teardown_shell()
{
  free(line);
}
