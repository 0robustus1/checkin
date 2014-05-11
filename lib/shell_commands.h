#ifndef SHELL_COMMANDS_H_OCCURED
#define SHELL_COMMANDS_H_OCCURED

#include "common.h"
#include "commands.h"
#include "shell.h"

struct tm * retrieve_now(struct tm *storage);
int handle_list();
int handle_start();
int handle_stop();

#endif /* SHELL_COMMANDS_H_OCCURED */

