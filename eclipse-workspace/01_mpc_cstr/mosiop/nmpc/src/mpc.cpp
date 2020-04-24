/*
 * mpc.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../inc/mpc.h"


/*
 * Extern functions to be called from Modelica environment.
 */
extern "C" void* createSolver( double tf, double N )
{
	printf("Create solve for  N=%i, tf=%.f, dt=%.3f\n", (int)N, (float)tf, (float)tf/N);

	MpcC* solver = new MpcC( (float)tf, (int)N);

	return (void*)solver;
}

extern "C" void destroySolver( void* solver )
{
	printf("Delete instance of solver\n");

	delete solver;
}

extern "C" double solve(void * vptr, double x1, double x2, double x3 )
{
	MpcC* solver_ptr = (MpcC*)vptr;

    return 0;
}




