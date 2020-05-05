/*
 * main.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */
#include "../../nmpc/inc/mpc.h"

int main( void )
{

	void * vptr = create_solver( 1, 2, 1 );

	casadi::DMDict res, arg = {{"x0",casadi::DM({0.1,0.3,0.4})}};

	((MpcC*)vptr)->solve(arg, res);

	destroy_solver( vptr );

	return 0;
}





