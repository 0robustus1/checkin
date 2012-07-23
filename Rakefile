require 'fileutils'
$flags = %w{-g}
$lflags = %w{-lsqlite3}
task :default => 'bin/checkin'

file 'bin/checkin' => %w{lib/checkin.c lib/checkin.h bin/timeslot.o} do |t|
  sh "gcc #{$lflags.join(' ')} #{$flags.join(' ')} -o #{t.name} #{t.prerequisites.join(' ')}"
end

file 'bin/timeslot.o' => %w{lib/timeslot.c lib/timeslot.h} do |t|
  sh "gcc #{$flags.join(' ')} -c -o #{t.name} #{t. prerequisites.first}"
end

