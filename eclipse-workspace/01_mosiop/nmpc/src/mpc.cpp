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
void* create_solver( double tf, double N, double K )
{
	printf("\n\033[1;36mInstantiate NLP solver.\033[0m\n\n" );

	MpcC* solver = new MpcC( (float)tf, (int)N, (int)K);

	return (void*)solver;
}

void destroy_solver( void* vptr )
{
	printf("\n\033[1;36mDelete instance of NLP solver\033[0m\n");

	if( vptr!=NULL )
		delete (MpcC*)vptr;
}

double solve(void * vptr, double x1, double x2, double x3 )
{
	MpcC* solver_ptr = (MpcC*)vptr;

	casadi::DMDict res, arg = {{"x0",casadi::DM({x1,x2,x3})}};

	solver_ptr->solve(arg, res);

    return 0;
}

void MpcC::solve(casadi::DMDict& arg, casadi::DMDict& res)
{
	casadi::DMDict nlp_res;
	nlp.solve(arg, nlp_res);
}





