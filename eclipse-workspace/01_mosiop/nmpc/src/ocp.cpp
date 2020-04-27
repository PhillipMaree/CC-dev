/*
 * ocp.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../inc/ocp.h"
#include <string>

/*
 * Ocp
 */
OcpC::OcpC( float tf )
{
	t = VmC("t",casadi::DM(1), casadi::DM(tf));
};

/*
 * Application
 */
CstrC::CstrC( float tf ):OcpC(tf)
{
	/*
	 * Process description
	 */
	name = "Acetylene hydrogenation CSTR process";

	/*
	 * Variable setup
	 */
	u = VmC("u", casadi::DM({1e-3}), casadi::DM({5.0}));
	y = VmC("y", casadi::DM({1e-3,1e-3,1e-3}), casadi::DM({1.0,1.0,1.0}));
	p = VmC("p", casadi::DM({1000,472,23}));

}

casadi::MX CstrC::L(void)
{
	casadi::MX x3 = y(2);
	return -x3;
}

casadi::MX CstrC::M(void)
{
	casadi::MX x3 = y(2);
	return -x3;
}

casadi::MX CstrC::f(void)
{

	casadi::MX x1 = y(0);
	casadi::MX x2 = y(1);
	casadi::MX x3 = y(2);

	casadi::MX u1 = u(0);

	auto sigma1 = p(0);
	auto sigma2 = p(1);
	auto beta = p(2);

	casadi::MX a = sigma1*x1*x2/(1+beta*x1);
	casadi::MX b = sigma2*pow(x2,0.5)*x3/(1+beta*x1);

	casadi::MX f1 = 1-x1-a;
	casadi::MX f2 = u1-x2-a-b;
	casadi::MX f3 = -x3+a-b;

	return vertcat(f1, f2, f3);

}

