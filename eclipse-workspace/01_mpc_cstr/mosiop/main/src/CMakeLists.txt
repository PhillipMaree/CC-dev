include_directories( ${PROJECT_SOURCE_DIR}/main/inc/ )

set( MAIN_SRC main.cpp debug.cpp )

add_executable( mosiop ${MAIN_SRC} )
