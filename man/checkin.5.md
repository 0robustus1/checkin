CHECKIN 5 "JULY 2012" "Manpages"
=======================================

NAME
----

checkin.cfg - checkin configuration file

SYNOPSIS
--------

*~/.config/checkin/checkin.cfg*

*~/.checkinrc* - not yet supported

DESCRIPTION
-----------

This configuration file is used to set user-specific settings
which are (usually) not supported by the commandline switches,
because including them would bloat the commandline.

Every setting belongs to one of two categories:

  *contract-specific*

  *independent*

Independent settings are user-specific global settings which are
are read in every mode and with every commandline-switch.
Contract-specific settings are a little bit more complicated. 
A contract is a specific time you work for someone, in my case these
times are usually very short (three to six months, after that a
new contract must be negotiated). For this time-period some 
administration specific values are relevant when one tries to
fill his or her timecard. For example the contract number. These
contract-specific settings are in effect whenever we work on
timeslots which belong to a contract (determined by time-period).

Now to the syntax of the configuration file.
A normal setting would be set by a line like this:

`key = value`. 

`[contract]` starts a new contract entry.

`;` is the comment-character.

### independent Settings

`verbose` - true/false (*false*) - corresponds to the `-v`-switch.
  

### contract-specific Settings

`starts` - "DD.MM.YYYY"/"MM/YYYY", required - sets a contract start-date.

`ends` - "DD.MM.YYYY"/"MM/YYYY", required - sets a contract end-date.

`contract` - text, optional - sets an identification for this contract (perhaps a transactionnumber)

`description` - text, optional - an optional description which has no impact on programbehaviour.


FILES
-----

*~/.config/checkin/checkin.cfg*
  Per user configuration file. See checkin(5) for further details.

*~/.checkinrc* 
  Alternative file - not yet supported.

*~/.config/checkin/times.db*
  User-specific timeslot database.

ENVIRONMENT
-----------

Some settings can also be set via environment variables.
A list of those variables will soon be available.


AUTHOR
------

Tim Reddehase <robustus@rightsrestricted.com>

SEE ALSO
--------

checkin(1), [checkin page](
http://rightsrestricted.com)
