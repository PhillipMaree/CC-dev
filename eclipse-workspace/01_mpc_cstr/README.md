####Getting started

cd build

cmake .. && make

To build other targets, choose from 

	add_executable(nmpc ${MPC_SRCS})
	add_library(nmpc SHARED ${MPC_SRCS})
	add_library(nmpc STATIC ${MPC_SRCS})