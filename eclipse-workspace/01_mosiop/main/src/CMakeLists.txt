include_directories( ${PROJECT_SOURCE_DIR}/main/inc/ )

set( MAIN_SRC main.cpp mpc.cpp )

include_directories( inc )

add_executable( main ${MAIN_SRC} )

target_link_libraries( main solver )