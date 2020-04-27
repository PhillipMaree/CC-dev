include_directories( ${PROJECT_SOURCE_DIR}/nmpc/inc/ )

set( NMPC_SRC mpc.cpp nlp.cpp ocp.cpp)

include_directories( inc )

find_library( CASADI_LIB libcasadi.so )

if( CASADI_LIB )
	add_library( mpc STATIC ${NMPC_SRC} )
	target_link_libraries( mpc ${CASADI_LIB} )
endif()



