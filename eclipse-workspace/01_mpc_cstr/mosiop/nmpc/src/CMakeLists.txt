include_directories( ${PROJECT_SOURCE_DIR}/nmpc/inc/ )

set( NMPC_SRC mpc.cpp nlp.cpp ocp.cpp solver.cpp )

add_library( nmpc STATIC ${NMPC_SRC} )



