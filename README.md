checkin - stupid timetracking
=============================

Checkin is a simple commandline tool to manage
time you spent doing something. I used it to
track my worked hours as a student assistant.


## Usage

For more information please look at the two markdown-manpages, in
the man-directory, or generate the corresponding manpages
with the rake-command.

`checkin` allows you to keep track of your worked time.
You can checkin so called **timeslots** to represent
given work hours. You can also list your worked time in a month
by hours for each day. 

<code> checkin -l [-f <em>config-file</em>] [-d <strong>date</strong>]</code>

<code> checkin -s [-f <em>config-file</em>] [-d <strong>date</strong>]</code>

<code> checkin [-d <strong>date</strong>] -b <strong>time</strong> -e <strong>time</time></code>

- `-b` **time**  
  Defines the given **time** as the starttime of a timeslot.
  Format is: `HH:MM`.
  Forces occurrence of `-e`. Assumes the current day if no date
  is specified by using the `-d`-switch.

- `-d` **date**  
  Assumes the given Date for the other switches.
  This switch support different formats dependend on which 
  it is used with. 
    when using `-l` *or* `-s`: `MM/YYYY`
    when using `-b` *and* `-e`: `DD.MM.YYYY`

- `-e` **time**  
  Defines the given **time** as the endtime of a timeslot.
  Format is: `HH:MM`.
  Forces occurrence of `-b`. Assumes the current day if no date
  is specified by using the `-d`-switch.

- `-f` *config-file*  
  Instead of using the default config-file (residing in ~/.config/checkin/checkin.cfg)
  another config-file as denoted by the argument is used.

- `-l`  
  Lists the worked time for each day in the current month. If another
  month is suggested by the `-d`-switch this month (and year)
  is used instead.

- `-s`  
  Prints the status of the current month. By default it only
  presents the worked hours and minutes of the current month
  (which is overriden by the `-d`-switch). If specified in a *config-file*
  it shows more useful stats.

- `-v`  
  Be more verbose...!

## Things to be done

- use a packaging system (like autotools)
- adding useful notes to timeslots
- editor-integration (like git-commit)
- other small gadgets to make living easier ;)


## License

([The MIT License][mit])

Copyright © 2013:

- [Tim Reddehase][1]

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[mit]: http://opensource.org/licenses/MIT
[1]: http://rightsrestricted.com
