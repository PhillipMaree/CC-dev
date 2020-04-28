/*
 * nlp.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../inc/nlp.h"
#include "../inc/debug.h"

/*
 * Collocation method initialization
 */

ColC::ColC( int degree, std::string method)
{
	int order = degree+1;

	K = degree;
	B = casadi::DM(1, order);
	C = casadi::DM(order, order);
	D = casadi::DM(1, order);

	std::vector<double> tau_root_{0};
	std::vector<double> taui=casadi::collocation_points(degree, method);
	tau_root_.insert( tau_root_.end(), taui.begin(), taui.end() );
	tau_root = casadi::DM( tau_root_ );

	casadi::MX tau = casadi::MX::sym("tau",1,1);
	casadi::MX x = casadi::MX::sym("x",1,1);

	for( int j=0; j<order; j++) {
		// construct polynomial for j-th collocation point
		casadi::MX lj = 1;

		// polynomial construction (see Biegler Ch10, 10.2.1 (10.4) p289)
		for( int k=0; k<order; k++ )
			if( j!=k )
				lj *= ( tau - tau_root(k) ) / ( tau_root(j) - tau_root(k) );
		casadi::Function F_lj = casadi::Function("F_lj",{tau},{lj});

		// coefficients continuity equations
		D(j) = F_lj(casadi::DM(1.0));

		// coefficients for collocation equations

		casadi::MX dlj = gradient(lj, tau);
		casadi::Function F_dlj = casadi::Function("F_dlj",{tau},{dlj});



		for( int k=0; k<order; k++ )
			C(j,k) = F_dlj(casadi::DM(tau_root(k)));


		// integral of the polynomial
		casadi::MXDict dae;
		dae["x"] = x;
		dae["t"] = tau;
		dae["ode"] = lj;

		casadi::Function I = casadi::integrator("I", "cvodes", dae );
		casadi::Function F_I = casadi::Function( "F_I", {tau}, {I( casadi::MXDict{{"x0", 0}} ).at("xf")} );

		B(j) = F_I(F_lj(casadi::DM(0.0)));
	}

	DEBUG(tau_root, "Tau Roots:");
	DEBUG(B, "B:");
	DEBUG(C, "C:");
	DEBUG(D, "D:");

};

/*
 * NLP formulation class functions
 */


