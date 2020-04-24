/*
 * ocp.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../inc/ocp.h"
#include <string>

/*
 * VarC
 */
template <class T>
void VarC<T>::init(  std::string name_, std::initializer_list<T> lb_, std::initializer_list<T> ub_ )
{
	name = name_;

	lb.set(lb_);
	ub.set(ub_);

	v = casadi::MX::sym(name, 1, lb.len());
}

/*
 * Ocp
 */
OcpC::OcpC( float tf )
{
	t.VarC("time", 1, tf);
};

virtual OcpC::~OcpC( void )
{
	t.~VarC();
	u.~VarC();
	y.~VarC();
	p.~VarC();
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
	u.VarC("u", {1e-3}, {5.0});
	y.VarC("y", {1e-3,1e-3,1e-3}, {1.0,1.0,1.0});
	p.VarC("p", {1000,472,23});
}

casadi::MX CstrC::L(void)
{
	casadi::MX x3 = y.v(2);
	return -x3;
}

casadi::MX CstrC::M(void)
{
	casadi::MX x3 = y.v(2);
	return -x3;
}

casadi::MX CstrC::f(void)
{

	casadi::MX x1 = y.v(0);
	casadi::MX x2 = y.v(1);
	casadi::MX x3 = y.v(2);

	casadi::MX u1 = u.v(0);

	float sigma1 = p.v(0);
	float sigma2 = p.v(1);
	float beta = p.v(2);

	casadi::MX a = sigma1*x1*x2/(1+beta*x1);
	casadi::MX b = sigma2*pow(x2,0.5)*x3/(1+beta*x1);

	casadi::MX f1 = 1-x1-a;
	casadi::MX f2 = u1-x2-a-b;
	casadi::MX f3 = -x3+a-b;

	return casadi::vertcat(f1, f2, f3);
}

