#ifndef SHELL_H_OCCURED
#define SHELL_H_OCCURED

#include "common.h"
#include "commands.h"
#include "shell_commands.h"

typedef bool (*shell_command_p)();

struct shell_tuple {
  char *keyword;
  shell_command_p shell_command;
};

typedef struct shell_tuple shell_tuple_t;
typedef shell_tuple_t* shell_tuple_p;


extern const char *in_line_delimiters;

void checkin_shell();

#endif /* SHELL_H_OCCURED */

