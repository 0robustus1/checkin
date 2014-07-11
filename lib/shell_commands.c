#include "shell_commands.h"

int extract_month_params(int *year, int *month);
void print_time(tm_p time, const char *end_with);
int ask_for_confirmation(char *confirm_for);
int answer_to_boolean(char *answer);

tm_p current_start = NULL;

tm_p retrieve_now(tm_p storage)
{
  time_t now_epoch;
  time(&now_epoch);
  return localtime_r(&now_epoch, storage);
}

bool handle_start()
{
  printf("encountered %s.\n", "start");
  if( !current_start ) {
    current_start = malloc( sizeof(tm_t) );
    current_start = retrieve_now(current_start);
    printf("Started Session at: ");
    print_time(current_start, "\n");
  } else {
    printf("Already running a session, since: ");
    print_time(current_start, "\n");
  }
  return true;
}

bool handle_stop()
{
  printf("encountered %s.\n", "stop");
  if( current_start ) {
    tm_p current_stop = malloc( sizeof(tm_t) );
    retrieve_now(current_stop);
    printf("Stopped...\n");
    printf("Session-Info:\n");
    printf("Start: ");
    print_time(current_start, "\n");
    printf("End: ");
    print_time(current_stop, "\n");

    int store = ask_for_confirmation("Store the timeslot?");

    if( store ) {
      printf("Ok, i will store the timeslot...\n");
      checkin_add(db_handler, current_start, current_stop);
    } else {
      printf("Storing of timeslot aborted.\n");
    }

    free(current_start);
    current_start = NULL;
    free(current_stop);
  } else {
    printf("There is no current session.\n");
  }
  return true;
}

bool handle_list()
{
  tm_t now;
  tm_p now_p = retrieve_now(&now);

  int *year_p = (int *) malloc( sizeof(int) );
  int *month_p = (int *) malloc( sizeof(int) );

  int success = extract_month_params(year_p, month_p);

  switch( success ) {
    case -1:
    case 0:
      free(year_p);
      free(month_p);
      month_p = NULL;
      year_p = NULL;
      break;
    case 1:
      free(year_p);
      year_p = NULL;
      break;
    default:
      break;
  }

  if( success ) {
    checkin_list(db_handler, now_p, year_p, month_p);
  }

  free(month_p);
  free(year_p);

  return true;
}

bool handle_exit()
{
  return false;
}

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
