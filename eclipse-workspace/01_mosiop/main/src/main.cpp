/*
 * main.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */
#include "../../nmpc/inc/debug.h"
#include "../../nmpc/inc/ocp.h"



int main( void )
{
	printf("MOSIOP\n");
	casadi::DM vm_ub({1.123,2,3});
	VmC a;
	a=VmC("P",casadi::DM({1.1,2.1,3.1}));


	DEBUG(a.ub(),"Test DM");
	DEBUG(a,"Test MX");


	return 0;
}



