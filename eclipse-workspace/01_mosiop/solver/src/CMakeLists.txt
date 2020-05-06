include_directories( ${PROJECT_SOURCE_DIR}/solver/inc/ )

set( SOLVER_SRC nlp.cpp ocp.cpp debug.cpp)

include_directories( inc )

find_library( CASADI_LIB libcasadi.so )

if( CASADI_LIB )
	add_library( solver SHARED ${SOLVER_SRC} )
	target_link_libraries( solver ${CASADI_LIB} )
endif()



