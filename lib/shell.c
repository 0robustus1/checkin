#include "shell.h"

const int max_line_length = 160;
const char *in_line_delimiters = " \n";
const char *prompt = "±> ";

char *line;

void setup_shell();
void teardown_shell();

int process_line();
void printNow();
struct tm * retrieve_now(struct tm *storage);
int extract_month_params(int *year, int *month);

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
    printNow();
  } else if( !strcmp(cmd, "stop") ) {
    printf("encountered %s.\n", cmd);
    printNow();
  } else if ( !strcmp(cmd, "list") ) {
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


  }

  fflush(NULL);
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

void printNow()
{
  time_t now_epoch;
  time(&now_epoch);
  struct tm *now = localtime(&now_epoch);
  char *time_string = (char *) malloc(20 * sizeof(char));
  strftime(time_string, 20, TIME_FORMAT, now);

  printf("%s\n", time_string);

  free(time_string);
}

struct tm * retrieve_now(struct tm *storage)
{
  time_t now_epoch;
  time(&now_epoch);
  return localtime_r(&now_epoch, storage);
}

void setup_shell()
{
  line = (char *) malloc(max_line_length * sizeof(char));
  initialize_database_connection();
}

void teardown_shell()
{
  free(line);
  kill_database_connection();
}
