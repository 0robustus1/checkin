include_directories(${CHECKIN_SOURCE_DIR}/lib)
link_directories(${CHECKIN_BINARY_DIR}/lib)

add_executable(checkin checkin.c)

set(CORELIBS ${SQLITE3_LIBRARIES})

add_library(timeslot timeslot.c)
add_library(database database.c)
add_library(shell shell.c)

set(LOCAL_LIBRARIES timeslot database shell)

target_link_libraries(checkin ${CORELIBS} ${LOCAL_LIBRARIES})
