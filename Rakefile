require 'fileutils'
$flags = %w{-lsqlite3}

file 'checkin' => ["checkin.c", "checkin.h"] do |t|
  sh "gcc #{$flags.join(' ')} -o #{t.name} #{t.prerequisites.join(' ')}"
end

task :default => 'checkin'
