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

ColC::ColC( int K_, std::string scheme_) : K(K_), scheme(scheme_), B(DM(1, K+1)), C(DM(K+1, K+1)), D(DM(1, K+1))
{

	std::vector<double> tau0{0};
	std::vector<double> taui=collocation_points(K, scheme);
	tau0.insert( tau0.end(), taui.begin(), taui.end() );

	// collocation points
	tau = DM( tau0 );

	MX t = MX::sym("t",1,1);
	MX x = MX::sym("x",1,1);

	for( int j=0; j<K+1; j++) {
		// construct polynomial for j-th collocation point
		MX lj = 1;

		// polynomial construction (see Biegler Ch10, 10.2.1 (10.4) p289)
		for( int k=0; k<K+1; k++ )
			if( j!=k )
				lj *= ( t - tau(k) ) / ( tau(j) - tau(k) );
		Function F_lj = Function("F_lj",{t},{lj});

		// coefficients continuity equations
		D(j) = F_lj(DM(1.0));

		MX dlj = gradient(lj, t);
		Function F_dlj = Function("F_dlj",{t},{dlj});

		for( int k=0; k<K+1; k++ )
			// coefficients for collocation equations
			C(j,k) = F_dlj(DM(tau(k)));

        Function I = integrator("I", "cvodes", {{"x",x},{"t",t},{"ode",lj}} , {{"reltol",1e-8}, {"abstol",1e-8}} );
		Function F_I = Function( "F_I", {t}, {I( MXDict{{"x0", 0}} ).at("xf")} );

		// integral of the polynomial
		B(j) = F_I(F_lj(DM(0.0)));

	}
};



/*
 * NLP formulation class functions
 */

NlpC::NlpC( float tf, int N_ ) :
		ColC( ),
		ocp( tf ),
		N(N_),
		h(tf/N),
		n(ocp.y.size().first),
		m(ocp.u.size().first),
		stats(n,m,N,K),
		offsets(K)
{
	/*
	 * initialize an empty NLP structure
	 */
	create();

	/*
	 * transcribe the OCP to NLP problem
	 */
	transcribe();

}

void NlpC::report( void )
{
	printf("NLP collocation scheme: %s of degree(order) %d(%d)\n", scheme.c_str(), K, K+1 );
	stats.report();

	DEBUG(tau, "Collocation points");
	DEBUG(B, "B");
	DEBUG(C, "C");
	DEBUG(D, "D");
}

void NlpC::create( void )
{

	for( int k=0; k<N; k++) {
		append( k+tau(0)*h );

		for( int j=1; j<K+1; j++ ) {
			append( k+tau(j)*h );
		}
	}

	append( N*h );
}

void NlpC::transcribe( void )
{
	/*
	 * MPC receding horizon for first N-1 stages
	 */
	for(int k=0; k<N-1; k++) {

		/*
		 * continuity boundary constraints
		 */


		/*
		 * residual constraints for collocation equations
		 */


		/*
		 * stage cost
		 */

	}

	/*
	 * MPC terminal stage N
	 */
}

void NlpC::append(const double& t)
{

}


void NlpC::append(const casadi::MX& J)
{

}

void NlpC::append(const casadi::MX& g, const casadi::DM& lbg, const casadi::DM& ubg)
{

}

void NlpC::append(const casadi::MX& z, const casadi::DM& lbz, const casadi::DM& ubz, const casadi::DM& z0)
{

}


