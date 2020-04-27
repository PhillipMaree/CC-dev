include_directories( ${PROJECT_SOURCE_DIR}/main/inc/ )

set( MAIN_SRC main.cpp )

include_directories( inc )

add_executable( mosiop ${MAIN_SRC} )

target_link_libraries( mosiop mpc )