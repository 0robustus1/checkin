include_directories(${CHECKIN_SOURCE_DIR}/lib)

add_executable(checkin checkin.c)

set(CORELIBS ${SQLITE3_LIBRARIES})

add_library(timeslot timeslot.c)
add_library(database database.c)
add_library(shell shell.c)
add_library(commands commands.c)
add_library(shell_commands shell_commands.c)


add_subdirectory(shell_commands)
get_directory_property(shell_commands_libraries DIRECTORY shell_commands local_libraries)


set(LOCAL_LIBRARIES timeslot database shell commands shell_commands)
set(LIBRARIES ${shell_commands_libraries} ${LOCAL_LIBRARIES})

target_link_libraries(checkin ${CORELIBS} ${LIBRARIES})
