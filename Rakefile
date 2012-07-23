require 'fileutils'
$flags = %w{-g}
$lflags = %w{-lsqlite3}
task :default => 'checkin'

file 'checkin' => %w{checkin.c checkin.h timeslot.o} do |t|
  sh "gcc #{$lflags.join(' ')} #{$flags.join(' ')} -o #{t.name} #{t.prerequisites.join(' ')}"
end

file 'timeslot.o' => %w{timeslot.c timeslot.h} do |t|
  sh "gcc #{$flags.join(' ')} -c -o #{t.name} #{t. prerequisites.first}"
end

