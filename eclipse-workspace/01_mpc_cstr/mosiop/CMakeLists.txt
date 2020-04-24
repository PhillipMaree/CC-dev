cmake_minimum_required(VERSION 3.10)

project( mosiop )

set( CMAKE_CXX_FLAGS "-fPIC" )

add_subdirectory(main)
#add_subdirectory(nmpc)

#target_link_libraries(mosiop nmpc)