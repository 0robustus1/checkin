#include "checkin.h"

const int true = 1;
const int false = 0;

void checkin_initialize();
void checkin_terminate(int exit_state);

const char * TIME_FORMAT   = "%Y-%m-%d %H:%M:00";

int *mode, *verbose, *b_opt_set, *d_opt_set, *e_opt_set;


int main(int argc, char *argv[])
{
  checkin_initialize();
  if( argc == 1 ) {
    printf("No option or keyword specified. Starting shell...\n");
    checkin_shell();
  }
  else if( argc > 1) {
    char *keyword = NULL;
    int arg_c = argc;
    char **arg_v = argv;
    /* not an option */
    if( argv[1][0] != '-' ) {
      keyword = argv[1];
      arg_c -= 1;
      arg_v = argv+1;
    }
    initialize_database_connection();
    handle_options(keyword, arg_c, arg_v);
  }
  checkin_terminate(0);
}

void checkin_initialize()
{
  db_open = malloc( sizeof(int) );
  *db_open = false;

  mode = malloc( sizeof(int) );
  *mode = CheckinNoMode;

  verbose = malloc( sizeof(int) );
  *verbose = DontBeVerbose;


  b_opt_set = malloc( sizeof(int) );
  *b_opt_set = false;

  d_opt_set = malloc( sizeof(int) );
  *d_opt_set = NoDateSet;

  e_opt_set = malloc( sizeof(int) );
  *e_opt_set = false;
}

void checkin_terminate(int exit_state)
{
  kill_database_connection();

  free(db_open);
  free(mode);
  free(verbose);
  free(b_opt_set);
  free(d_opt_set);
  free(e_opt_set);

  exit(exit_state);
}

void out(char *output)
{
  if( *verbose )
    puts(output);
}

void handle_options(char *keyword, int argc, char **argv)
{
  int current_opt;

  time_t now_epoch;
  time(&now_epoch);
  struct tm *now = localtime(&now_epoch);

  int year = now->tm_year + 1900;
  int month = now->tm_mon + 1;
  int day = now->tm_mday;
  int beginsHour = now->tm_hour - 1;
  int beginsMinute = now->tm_min;
  int endsHour = now->tm_hour;
  int endsMinute = now->tm_min;

  while ((current_opt = getopt (argc, argv, "lsd:b:e:v")) != -1) {
    switch(current_opt) {
      case 'l':
        *mode = CheckinListing;
        break;
      case 's':
        *mode = CheckinStatus;
        break;
      case 'd':
        if( sscanf(optarg, "%d.%d.%d", &day, &month, &year) != 3 ) {
          if( sscanf(optarg, "%d/%d", &month, &year) != 2 ) {
            out("-d switch used in the wrong way...");
            checkin_terminate(1);
          } else {
            *d_opt_set = DateWithoutDaySet;
          }
        } else {
          *d_opt_set = DateSet;
        }
        break;
      case 'b':
        sscanf(optarg, "%d:%d", &beginsHour, &beginsMinute);
        *b_opt_set = true;
        break;
      case 'e':
        sscanf(optarg, "%d:%d", &endsHour, &endsMinute);
        *e_opt_set = true;
        break;
      case 'v':
        *verbose = BeVerbose;
        break;
      default:
        abort ();
    }
  }
  if( *mode != CheckinNoMode ) {
    if( *d_opt_set==DateSet )
      out("Date set, ignoring day-value...");
    if( *mode == CheckinListing )
      checkin_list(db_handler, now, (*d_opt_set) ? &year : NULL, (*d_opt_set) ? &month : NULL);
    else if( *mode == CheckinStatus )
      checkin_status(db_handler, now, (*d_opt_set) ? &year : NULL, (*d_opt_set) ? &month : NULL);
    kill_database_connection();
    checkin_terminate(0);
  } else {
    if( !(*b_opt_set && *e_opt_set) ) {
      out("Just one of {-b,-e} set, you need to set both...");
      checkin_terminate(1);
    }
    if( *d_opt_set==DateWithoutDaySet ) {
      out("You need to use the DD.MM.YYY format when adding...");
      checkin_terminate(1);
    }
    if( !*d_opt_set ) {
      out("No date set, using todays date...");
      year = now->tm_year+1900;
      month = now->tm_mon+1;
      day = now->tm_mday;
    }
    struct tm begins = {
      .tm_year  = year-1900,
      .tm_mon   = month-1,
      .tm_mday  = day,
      .tm_hour  = beginsHour-1,
      .tm_min   = beginsMinute
    };
    struct tm ends = {
      .tm_year  = year-1900,
      .tm_mon   = month-1,
      .tm_mday  = day,
      .tm_hour  = endsHour-1,
      .tm_min   = endsMinute
    };
    mktime(&begins);
    mktime(&ends);
    int success = checkin_add(db_handler, &begins, &ends);
    checkin_terminate(success);
  }
}
