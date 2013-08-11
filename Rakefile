require 'fileutils'
require 'rake/clean'

CLEAN.include('bin/*.o')
CLOBBER.include('bin/checkin')

# clang -lsqlite3 -Wall -pedantic -std=c11 lib/checkin.c bin/timeslot.o -o bin/checkin_clang 
$COMPILER = "clang"
$flags = %w{-g -Wall -std=c11 -pedantic}
$lflags = %w{-lsqlite3}
directory "bin"
directory "man"
desc "build checkin..."
task :default => %w{bin bin/checkin}

file 'bin/checkin' => %w{lib/checkin.c bin/timeslot.o} do |t|
  cmd = "#{$COMPILER} #{$lflags.join(' ')} #{$flags.join(' ')} -o #{t.name} #{t.prerequisites.join(' ')}"
  puts cmd
  sh cmd
end

file 'bin/timeslot.o' => %w{lib/timeslot.c} do |t|
  cmd = "#{$COMPILER} #{$flags.join(' ')} -c -o #{t.name} #{t. prerequisites.first}"
  puts cmd
  sh cmd
end

desc "compile manpages..."
task :manpages => Dir.glob("man/*.*.md").map{|f| f.sub(/md$/,'gz')}

Dir.glob("man/*.*.md").map{|f| f.sub(/md$/,'gz')}.each do |f|
  md_file = f.sub(/gz$/,'md')
  file f => md_file do 
    manpage_file = f.sub(/\.gz$/,'')
    sh "md2man-roff #{md_file} > #{manpage_file}"
    sh "gzip --force #{manpage_file}"
  end
end
