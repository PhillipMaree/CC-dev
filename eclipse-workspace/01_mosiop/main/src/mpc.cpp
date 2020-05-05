/*
 * mpc.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../inc/mpc.h"

void MpcC::solve(casadi::DMDict& arg, casadi::DMDict& res)
{
	casadi::DMDict nlp_res;
	nlp.solve(arg, nlp_res);

	// TODO some closed-loop simulation
	res = nlp_res;
}





