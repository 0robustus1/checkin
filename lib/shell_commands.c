#include "shell_commands.h"

int extract_month_params(int *year, int *month);
void print_time(struct tm *time, const char *end_with);

struct tm *current_start = NULL;

struct tm * retrieve_now(struct tm *storage)
{
  time_t now_epoch;
  time(&now_epoch);
  return localtime_r(&now_epoch, storage);
}

int handle_start()
{
  printf("encountered %s.\n", "start");
  if( !current_start ) {
    current_start = (struct tm *) malloc( sizeof(struct tm) );
    current_start = retrieve_now(current_start);
    printf("Started Session at: ");
    print_time(current_start, "\n");
  } else {
    printf("Already running a session, since: ");
    print_time(current_start, "\n");
  }
  return true;
}

int handle_stop()
{
  printf("encountered %s.\n", "stop");
  if( current_start ) {
    struct tm *current_stop = (struct tm *) malloc( sizeof(struct tm) );
    retrieve_now(current_stop);
    printf("Stopped...\n");
    printf("Session-Info:\n");
    printf("Start: ");
    print_time(current_start, "\n");
    printf("End: ");
    print_time(current_stop, "\n");
    free(current_start);
    current_start = NULL;
    free(current_stop);
  } else {
    printf("There is no current session.\n");
  }
  return true;
}

int handle_list()
{
  struct tm now;
  struct tm *now_p = retrieve_now(&now);

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

void print_time(struct tm *time, const char *end_with)
{
  char *time_string = (char *) malloc(20 * sizeof(char));
  strftime(time_string, 20, TIME_FORMAT, time);

  printf("%s%s", time_string, end_with);

  free(time_string);
}
