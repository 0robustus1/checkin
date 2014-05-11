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

    // First part of month/year syntax
    char *param = strtok(NULL, "/ \n");
    if( param ) {
      int month;
      if( sscanf(param, "%d", &month) != 1) {
        printf("Expected valid `month/year` or `month` syntax.");
      } else {
        param = strtok(NULL, in_line_delimiters);
        if( param ) {
          int year;
          if( sscanf(param, "%d", &year) != 1) {
            printf("Expected valid `month/year` syntax.");
          } else {
            checkin_list(db_handler, now_p, &year, &month);
          }
        } else {
          checkin_list(db_handler, now_p, NULL, &month);
        }
      }
    } else {
      checkin_list(db_handler, now_p, NULL, NULL);
    }
  }

  fflush(NULL);
  return true;
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
