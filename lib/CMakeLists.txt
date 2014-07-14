include_directories(${CHECKIN_SOURCE_DIR}/lib)
link_directories(${CHECKIN_BINARY_DIR}/lib)

add_executable(checkin checkin.c)

set(CORELIBS ${SQLITE3_LIBRARIES})

add_library(timeslot timeslot.c)
add_library(database database.c)
add_library(shell shell.c)
add_library(commands commands.c)
add_library(shell_commands shell_commands.c)


add_subdirectory(shell_commands)

set(LOCAL_LIBRARIES timeslot database shell commands shell_commands shell_commands_start shell_commands_stop shell_commands_list shell_commands_exit)

target_link_libraries(checkin ${CORELIBS} ${LOCAL_LIBRARIES})
