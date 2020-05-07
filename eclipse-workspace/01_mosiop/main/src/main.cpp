/*
 * main.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */
#include "../inc/plot.h"
#include "../inc/mpc.h"

void TEST_EXTERN_NLP_OPENLOOP( void );
void TEST_LOCAL_MPC_OPENLOOP( void );
void TEST_LOCAL_MPC_CLOSEDLOOP( void );

static const float h=0.3;   // sampling rate
static const int N = 10;    // prediction horizon
static const int tf = 10;   // closed-loop final simulation time

int main( void )
{
	TEST_EXTERN_NLP_OPENLOOP();
	//TEST_LOCAL_MPC_CLOSEDLOOP();

	return 0;
}

void TEST_EXTERN_NLP_OPENLOOP( void )
{

	void * vptr = create_solver( h, N );

	casadi::DMDict arg = {{"x0",casadi::DM({0.1,0.1,0.1})}};

	double res = solve(vptr, 0.1,0.1,0.1);

	DEBUG(res, "Extern C u*:");

	destroy_solver( vptr );
}

void TEST_LOCAL_MPC_OPENLOOP( void )
{
	MpcC mpc( h, N, tf );

	casadi::DMDict res, arg = {{"x0",casadi::DM({0.1,0.1,0.1})}};

	res = mpc.solve( arg, false );

	FigC fig;
	fig.title("Open-loop CSTR MPC solution");
	fig.plot( res["t"], res["x"], res["u"]);
	fig.show();
}

void TEST_LOCAL_MPC_CLOSEDLOOP( void )
{
	MpcC mpc( h, N, tf );

	casadi::DMDict res, arg = {{"x0",casadi::DM({0.1,0.1,0.1})}};

	res = mpc.solve( arg );

	FigC fig;
	fig.title("Closed-loop CSTR MPC solution");
	fig.plot( res["t"], res["x"], res["u"]);
	fig.show();
}





