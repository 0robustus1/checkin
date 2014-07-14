#ifndef SHELL_COMMANDS_H_OCCURED
#define SHELL_COMMANDS_H_OCCURED

#include "common.h"
#include "commands.h"
#include "shell.h"

extern tm_p current_start;
extern const int margin_minutes;

int extract_month_params(int *year, int *month);
void print_time(tm_p time, const char *end_with);
int ask_for_confirmation(char *confirm_for);
int answer_to_boolean(char *answer);

bool handle_list();
bool handle_start();
bool handle_stop();
bool handle_exit();

#endif /* SHELL_COMMANDS_H_OCCURED */

