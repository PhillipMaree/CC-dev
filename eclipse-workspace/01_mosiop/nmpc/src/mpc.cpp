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
	printf("Create solve for  N=%i, tf=%.f, h=%.3f\n", (int)N, (float)tf, (float)tf/N);

	MpcC* solver = new MpcC( (float)tf, (int)N);

	return (void*)solver;
}

extern "C" void destroySolver( void* vptr )
{
	printf("Delete instance of solver\n");

	if( vptr!=NULL )
		delete (MpcC*)vptr;
}

extern "C" double solve(void * vptr, double x1, double x2, double x3 )
{
	MpcC* solver_ptr = (MpcC*)vptr;

    return 0;
}




