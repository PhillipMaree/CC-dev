/*
 * main.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */
#include "../inc/mpc.h"

void TEST_EXTERN_NLP_OPENLOOP( void );
void TEST_LOCAL_MPC_CLOSEDLOOP( void );

static const float h=0.3;
static const int N = 10;
static const int K = 3;

int main( void )
{
	TEST_EXTERN_NLP_OPENLOOP();

	return 0;
}

void TEST_EXTERN_NLP_OPENLOOP( void )
{

	void * vptr = create_solver( h, N, K );

	casadi::DMDict res, arg = {{"x0",casadi::DM({0.1,0.1,0.1})}};

	((NlpC*)vptr)->solve(arg, res);

	destroy_solver( vptr );
}

void TEST_LOCAL_MPC_CLOSEDLOOP( void )
{
	MpcC mpc( h, N, K );

	casadi::DMDict res, arg = {{"x0",casadi::DM({0.1,0.1,0.1})}};

	mpc.solve(arg, res);
}





