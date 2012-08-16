require 'fileutils'
require 'rake/clean'

CLEAN.include('bin/*.o')
CLOBBER.include('bin/checkin')

$GCC = "gcc-mp-4.7"
$flags = %w{-g -Wall}
$lflags = %w{-lsqlite3}
directory "bin"
directory "man"
desc "build checkin..."
task :default => %w{bin bin/checkin}

file 'bin/checkin' => %w{lib/checkin.c lib/checkin.h bin/timeslot.o} do |t|
  sh "#{$GCC} #{$lflags.join(' ')} #{$flags.join(' ')} -o #{t.name} #{t.prerequisites.join(' ')}"
end

file 'bin/timeslot.o' => %w{lib/timeslot.c lib/timeslot.h} do |t|
  sh "#{$GCC} #{$flags.join(' ')} -c -o #{t.name} #{t. prerequisites.first}"
end

desc "compile manpages..."
task :manpages => Dir.glob("man/*.*.md").map{|f| f.sub(/md$/,'gz')}

Dir.glob("man/*.*.md").map{|f| f.sub(/md$/,'gz')}.each do |f|
  md_file = f.sub(/gz$/,'md')
  file f => md_file do 
    manpage_file = f.sub(/\.gz$/,'')
    sh "md2man #{md_file} > #{manpage_file}"
    sh "gzip --force #{manpage_file}"
  end
end
