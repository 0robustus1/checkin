#include "shell_commands.h"

int extract_month_params(int *year, int *month);

struct tm * retrieve_now(struct tm *storage)
{
  time_t now_epoch;
  time(&now_epoch);
  return localtime_r(&now_epoch, storage);
}

int handle_start()
{
  printf("encountered %s.\n", "start");
  printNow();
  return true;
}

int handle_stop()
{
  printf("encountered %s.\n", "stop");
  printNow();
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
