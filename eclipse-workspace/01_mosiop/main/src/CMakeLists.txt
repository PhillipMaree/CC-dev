include_directories( ${PROJECT_SOURCE_DIR}/main/inc/ )

set( MAIN_SRC main.cpp mpc.cpp )

include_directories( inc )

add_executable( main ${MAIN_SRC} )

find_package( PythonLibs 3.6.9 )

target_include_directories(main PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(main solver ${PYTHON_LIBRARIES})