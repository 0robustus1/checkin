#include "shell_commands.h"

bool handle_list()
{
  tm_p now_p = now();

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
  tm_destroy(now_p);

  return true;
}
