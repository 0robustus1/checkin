#include "shell.h"

const int max_line_length = 160;
char *line;

void setup_shell();
void teardown_shell();

void checkin_shell()
{
  setup_shell();

  while(true) {
    printf("±> ");
    if (!fgets(line, max_line_length, stdin)) break;
    printf(line, "\n");
  }

  teardown_shell();
}

void setup_shell()
{
  line = (char *) malloc(max_line_length * sizeof(char));
}

void teardown_shell()
{
  free(line);
}
