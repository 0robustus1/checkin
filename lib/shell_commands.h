#ifndef SHELL_COMMANDS_H_OCCURED
#define SHELL_COMMANDS_H_OCCURED

#include "common.h"
#include "commands.h"
#include "shell.h"

tm_p retrieve_now(tm_p storage);
bool handle_list();
bool handle_start();
bool handle_stop();
bool handle_exit();

#endif /* SHELL_COMMANDS_H_OCCURED */

