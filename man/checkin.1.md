CHECKIN 1 "JULY 2012" "Manpages"
=======================================

NAME
----

checkin - checking in your worked time

SYNOPSIS
--------

`checkin` `-l` [`-f` *config-file* ] [`-d` **date**]

`checkin` `-s` [`-f` *config-file* ] [`-d` **date**]

`checkin` [`-d` **date**] `-b` **time** `-e` **time**

DESCRIPTION
-----------

`checkin` allows you to keep track of your worked time.
You can checkin so called **timeslots** to represent
given work hours. You can also list your worked time in a month
by hours for each day. 

OPTIONS
-------

`-b` **time**
    Defines the given **time** as the starttime of a timeslot.
    Format is: `HH:MM`.
    Forces occurrence of `-e`. Assumes the current day if no date
    is specified by using the `-d`-switch.

`-d` **date**
    Assumes the given Date for the other switches.
    This switch support different formats dependend on which 
    it is used with. 
      when using `-l` *or* `-s`: `MM/YYYY`
      when using `-b` *and* `-e`: `DD.MM.YYYY`

`-e` **time**
    Defines the given **time** as the endtime of a timeslot.
    Format is: `HH:MM`.
    Forces occurrence of `-b`. Assumes the current day if no date
    is specified by using the `-d`-switch.

`-f` *config-file*
    Instead of using the default config-file (residing in ~/.config/checkin/checkin.cfg)
    another config-file as denoted by the argument is used.

`-l`
    Lists the worked time for each day in the current month. If another
    month is suggested by the `-d`-switch this month (and year)
    is used instead.

`-s`
    Prints the status of the current month. By default it only
    presents the worked hours and minutes of the current month
    (which is overriden by the `-d`-switch). If specified in a *config-file*
    it shows more useful stats.

`-v`
    Be more verbose...!


FILES
-----

*~/.config/checkin/checkin.cfg*
  Per user configuration file. See checkin(5) for further details.

ENVIRONMENT
-----------

AUTHOR
------

Tim Reddehase <robustus@rightsrestricted.de>

SEE ALSO
--------

checkin(5), [checkin page](
http://rightsrestricted.de/software/checkin)
