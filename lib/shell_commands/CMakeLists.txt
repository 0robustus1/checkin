include_directories(${CHECKIN_SOURCE_DIR}/lib/shell_commands)
link_directories(${CHECKIN_BINARY_DIR}/lib/shell_commands)

add_library(shell_commands_start start.c)
add_library(shell_commands_stop stop.c)
add_library(shell_commands_list list.c)
add_library(shell_commands_exit exit.c)
add_library(shell_commands_report report.c)

set_property(DIRECTORY PROPERTY local_libraries shell_commands_start shell_commands_stop shell_commands_list shell_commands_exit shell_commands_report)
