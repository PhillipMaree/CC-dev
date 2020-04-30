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
		ocp( tf ),                // tf = h*N
		N(N_),                    // prediction horizon stages for MPC
		h(tf/N),                  // time length for each MPC stage
		n(ocp.y.size().first),    // number ofg
		m(ocp.u.size().first),
		stats(n,m,N,K),
		offsets(K)
{
	structure();
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

void NlpC::structure( void )
{
	for( int k=0; k<N; k++ ) {

		dm_nlp["t"].append( DM({( k+tau(0) )*h}) );

		mx_nlp["x"].append( MX::sym( ocp.y.name()+"_"+std::to_string(k), ocp.y.sparsity() ) );
		dm_nlp["lbx"].append( ocp.y.lbx() );
		dm_nlp["ubx"].append( ocp.y.ubx() );
		dm_nlp["x0"].append( ocp.y.x0() );

		for( int j=1; j<K+1; j++ ) {

			dm_nlp["t"].append( DM({( k+tau(j) )*h}) );

			mx_nlp["x"].append( MX::sym( ocp.y.name()+"_"+std::to_string(k)+"_"+std::to_string(j), ocp.y.sparsity() )  );
			dm_nlp["lbx"].append( ocp.y.lbx() );
			dm_nlp["ubx"].append( ocp.y.ubx() );
			dm_nlp["x0"].append( ocp.y.x0() );
		}

		mx_nlp["x"].append( MX::sym( ocp.u.name()+"_"+std::to_string(k), ocp.u.sparsity() ) );
		dm_nlp["lbx"].append( ocp.u.lbx() );
		dm_nlp["ubx"].append( ocp.u.ubx() );
		dm_nlp["x0"].append( ocp.u.x0() );
	}
	dm_nlp["t"].append( DM({N*h}) );

	mx_nlp["x"].append( MX::sym( ocp.y.name()+"_"+std::to_string(N), ocp.y.sparsity() )  );
	dm_nlp["lbx"].append( ocp.y.lbx() );
	dm_nlp["ubx"].append( ocp.y.ubx() );
	dm_nlp["x0"].append( ocp.y.x0() );


	DEBUG( dm_nlp["t"].eval(), "t");
	DEBUG( dm_nlp["ubx"].eval(), "ubx");
	DEBUG( mx_nlp["x"].eval(), "x");
	//DEBUG( mx_nlp["x"].eval().T(), "x");
	//DEBUG( dm_nlp["x0"].eval().T(), "x0");


}

void NlpC::transcribe( void )
{

}

