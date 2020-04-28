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
using namespace casadi;

ColC::ColC( int degree, std::string method) : K(degree), B(DM(1, K+1)), C(DM(K+1, K+1)), D(DM(1, K+1))
{

	std::vector<double> tau_root_{0};
	std::vector<double> taui=collocation_points(degree, method);
	tau_root_.insert( tau_root_.end(), taui.begin(), taui.end() );

	// collocation points
	tau_root = DM( tau_root_ );

	MX tau = MX::sym("tau",1,1);
	MX x = MX::sym("x",1,1);

	for( int j=0; j<K+1; j++) {
		// construct polynomial for j-th collocation point
		MX lj = 1;

		// polynomial construction (see Biegler Ch10, 10.2.1 (10.4) p289)
		for( int k=0; k<K+1; k++ )
			if( j!=k )
				lj *= ( tau - tau_root(k) ) / ( tau_root(j) - tau_root(k) );
		Function F_lj = Function("F_lj",{tau},{lj});

		// coefficients continuity equations
		D(j) = F_lj(DM(1.0));

		MX dlj = gradient(lj, tau);
		Function F_dlj = Function("F_dlj",{tau},{dlj});

		for( int k=0; k<K+1; k++ )
			// coefficients for collocation equations
			C(j,k) = F_dlj(DM(tau_root(k)));

        Function I = integrator("I", "cvodes", {{"x",x},{"t",tau},{"ode",lj}} , {{"reltol",1e-8}, {"abstol",1e-8}} );
		Function F_I = Function( "F_I", {tau}, {I( MXDict{{"x0", 0}} ).at("xf")} );

		// integral of the polynomial
		B(j) = F_I(F_lj(DM(0.0)));

	}

	DEBUG(tau_root, "Tau Roots:");
	DEBUG(B, "B:");
	DEBUG(C, "C:");
	DEBUG(D, "D:");

};

/*
 * NLP formulation class functions
 */


