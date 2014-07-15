#include "shell_commands.h"

tm_p current_start = NULL;
const int margin_minutes = 15;

/*
 * Returns false (zero), if errors were encountered.
 * Returns 1 if number of arguments is 1 (only month).
 * Returns 2 if number of arguments is 2 (month and year).
 * Returns -1 if no arguments were encountered.
 */
int extract_month_params(int *year_p, int *month_p)
{
  // First part of month/year syntax
  char *param = strtok(NULL, "/ \n");

  if( param ) {
    if( sscanf(param, "%d", month_p) != 1) {
      printf("Expected valid `month/year` or `month` syntax.");
      return false;
    } else {
      param = strtok(NULL, in_line_delimiters);
      if( param ) {
        if( sscanf(param, "%d", year_p) != 1) {
          printf("Expected valid `month/year` syntax.");
          return false;
        } else {
          return 2;
        }
      } else {
        return 1;
      }
    }
  } else {
    return -1;
  }
}

void print_time(tm_p time, const char *end_with)
{
  char *time_string = (char *) malloc(20 * sizeof(char));
  strftime(time_string, 20, TIME_FORMAT, time);

  printf("%s%s", time_string, end_with);

  free(time_string);
}

/*
 * Asks for confirmation.
 * Returns boolean value respective to yes/no response.
 * confirm_for: used as a message to user
 */
int ask_for_confirmation(char *confirm_for)
{
  int max_answer_length = 4;
  int unanswered = true;
  int boolean_answer = false;
  char *raw_answer = (char *) malloc(max_answer_length * sizeof(char));
  while(unanswered) {
    if( confirm_for ) {
      printf("%s\n", confirm_for);
    }
    printf("Confirm y/n: ");
    fflush(NULL);
    if( fgets(raw_answer, max_answer_length, stdin) ) {
      char *answer = strtok(raw_answer, in_line_delimiters);
      boolean_answer = answer_to_boolean(answer);
      if( boolean_answer == -1 ) {
        printf("Please provide either 'y' or 'n'.\n");
      } else {
        unanswered = false;
      }
    } else {
      break;
    }
  }
  free(raw_answer);
  return boolean_answer;
}

/*
 * Converts an answer-string into a boolean response.
 * "y" or "yes" will yield true.
 * "n" or "no" will yield false.
 * Every other value of answer will yield -1, which
 * denotes an invalid answer.
 */
int answer_to_boolean(char *answer) {
  int boolean = false;
  if( !answer ) return -1;
  if( !strcmp(answer, "y") || !strcmp(answer, "yes") ) {
    boolean = true;
  } else if( !strcmp(answer, "n") || !strcmp(answer, "no") ) {
    boolean = false;
  } else {
    boolean = -1;
  }
  return boolean;
}
