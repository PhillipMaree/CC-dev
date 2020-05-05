/*
 * nlp.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../../solver/inc/nlp.h"

#include "../inc/debug.h"

/*
 * Extern NLP solver interface
 */
void* create_solver( double h, double N, double K )
{
	printf("\n\033[1;36mInstantiate NLP solver.\033[0m\n\n" );

	NlpC* solver = new NlpC( (float)h, (int)N, (int)K);

	return (void*)solver;
}

void destroy_solver( void* vptr )
{
	printf("\n\033[1;36mDelete instance of NLP solver\033[0m\n");

	if( vptr!=NULL )
		delete (NlpC*)vptr;
}

double solve(void * vptr, double x1, double x2, double x3 )
{
	NlpC* solver_ptr = (NlpC*)vptr;

	casadi::DMDict res, arg = {{"x0",casadi::DM({x1,x2,x3})}};

	solver_ptr->solve(arg, res);

    return 0;
}

/*
 * Collocation method initialization
 */
using namespace casadi;

ColC::ColC( int K_, std::string scheme_) : K(K_), scheme(scheme_)
{
    B = DM(1, K+1);
    C = DM(K+1, K+1);
    D = DM(1, K+1);

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

	DEBUG(B,"Col B");
	DEBUG(C,"Col C");

};

template <typename T>
void DictC<T>::ProxySt::operator+=(T val) {
	if (vec.empty())
		vec.push_back( val );
	else
		vec.front() += val;
}

template <typename T>
typename DictC<T>::ProxySt DictC<T>::operator[](std::string key)
{
	itr = T_dict.find( key );
	if( itr != T_dict.end() )
		return ProxySt( itr->second ) ;
	else
		return ProxySt( T_dict[key] );
}

void CasadiFnC::link_fn( void )
{
	if( appOcp->l().sparsity().density() ) {
		casadi::Function l = casadi::Function( "fn_l", { appOcp->t, appOcp->y, appOcp->u }, {appOcp->l()}, {"t", "y", "u"}, {"l"} ) ;
		fn_map.insert( std::pair<std::string, casadi::Function>("l", l ));
	}

	if( appOcp->m().sparsity().density() ) {
		casadi::Function m = casadi::Function( "fn_m", { appOcp->t, appOcp->y, appOcp->u }, {appOcp->m()}, {"t", "y", "u"}, {"m"} ) ;
		fn_map.insert( std::pair<std::string, casadi::Function>("m", m ));
	}

	if( appOcp->f().sparsity().density() ) {
		casadi::Function fn_f = casadi::Function( "fn_f", { appOcp->t, appOcp->y, appOcp->u }, {appOcp->f()}, {"t", "y", "u"}, {"f"} ) ;
		fn_map.insert( std::pair<std::string, casadi::Function>("f", fn_f ));
	}

	if( appOcp->g().sparsity().density() ) {
		casadi::Function fn_g = casadi::Function( "fn_g", { appOcp->t, appOcp->y, appOcp->u }, {appOcp->g()}, {"t", "y", "u"}, {"g"} ) ;
		fn_map.insert( std::pair<std::string, casadi::Function>("g", fn_g ));
	}

	if( appOcp->h().sparsity().density() ) {
		casadi::Function fn_h = casadi::Function( "fn_h", { appOcp->t, appOcp->y, appOcp->u }, {appOcp->h()}, {"t", "y", "u"}, {"h"} ) ;
		fn_map.insert( std::pair<std::string, casadi::Function>("h", fn_h ));
	}
}

CasadiFnC::ProxySt CasadiFnC::operator[](std::string fn_name)
{
	itr = fn_map.find( fn_name );
	if( itr != fn_map.end() )
		return ProxySt( fn_name, itr->second  ) ;
	else
		return ProxySt();
}

/*
 * NLP formulation class functions
 */

NlpC::NlpC( float h_, int N_, int K_ ) :
		ColC( K_, "legendre"), appOcp( h_*N_ ), casadiFn(&appOcp),
		tf(h_*N_), h(h_), N(N_),
		n(appOcp.y.size1()), m(appOcp.u.size1()),
		y_offset(0), c_offset(1), u_offset(1+K_), stage_offset(2+K_),
		nlp_u_var_n(N*m), nlp_y_var_n((N+1)*n), nlp_c_var_n(N*K*n), nlp_g_var_n(N*(1+K)*n), nlp_t_var_n(nlp_u_var_n+nlp_y_var_n+nlp_c_var_n)
{

	build_nlp_struct();
	transcribe_nlp();

	report();

	DEBUG(y_offset,"y_offset");
	DEBUG(c_offset,"c_offset");
	DEBUG(u_offset,"u_offset");
	DEBUG(stage_offset,"stage_offset");
}

void NlpC::solve(casadi::DMDict& arg, casadi::DMDict& res)
{
	// set initiliza conditions
	arg_nlp["x0"].set(arg["x0"], true, Slice(0,arg["x0"].size1(),1), Slice());
	arg_nlp["lbx"].set(arg["x0"], true, Slice(0,arg["x0"].size1(),1), Slice());
	arg_nlp["ubx"].set(arg["x0"], true, Slice(0,arg["x0"].size1(),1), Slice());

	// solve NLP problem
	casadi::DMDict res_nlp = solver( arg_nlp );

	// NLP extracted optimal open-loop results
	res["t"] = dm_nlp["t"].concatenate().T();
	res["u"] = DM(m,N);
	res["x"] = DM(n,(1+K)*N + 1);

	// extraction
	for( int k=0;k<N; k++) {

		const int k_offset = k*( (K+1)*n +m );

		// extract control
		res["u"].set( res_nlp["x"]( Slice( k_offset +(K+1)*n, k_offset +(K+1)*n + m , 1 ) ),true,Slice(), Slice(k,k+1,1)  );

		// extract states with collocation points
		for( int j=0; j<K+1; j++)
			res["x"].set( res_nlp["x"]( Slice( k_offset +j*n, k_offset +(j+1)*n, 1 ) ), true, Slice(0,n,1), Slice(k*(K+1)+j,k*(K+1)+j+1,1));

	}
	res["x"].set( res_nlp["x"]( Slice( ((K+1)*n + m)*N, res_nlp["x"].size1(), 1 ) ), true, Slice(0,n,1), Slice(N*(K+1),N*(K+1)+1,1) );

	DEBUG(res_nlp["x"].T() ,"NLP x");
	DEBUG(res["t"],"t");
	DEBUG(res["u"],"MPC u");
	DEBUG(res["x"],"MPC x");

}

void NlpC::report( void )
{

	printf( "\033[1;34m\nMOdelling, SImulation & OPtimization (MOSIOP):\n\n"
			"\033[0;34m   target = %s\n"
			"   tf     = %.1f (OCP final time horizon optimization)\n"
			"   N      = %d   (MPC prediction horizon stages)\n"
			"   h      = %.3f (MPC stage step size)\n\n"
			"\033[1;34mNLP configuration (direct-collocation):\n\n"
			"\033[0;34m   scheme = %s\n"
			"   K      = %d (degree)\n"
			"   nlp_t  = %d (total of NLP variables)\n"
			"   nlp_y  = %d (differential variables)\n"
			"   nlp_u  = %d (piece-wise constant control variables)\n"
			"   nlp_g  = %d (residual constraints on collocation equations)\n"
			"   nlp_c  = %d (collocation variables)\033[0m\n\n", appOcp.name.c_str(), tf, N, (tf/N), scheme.c_str(), K,
			nlp_t_var_n,
			nlp_y_var_n,
			nlp_u_var_n,
			nlp_g_var_n,
			nlp_c_var_n );

}

void NlpC::build_nlp_struct( void )
{
	/*
	 * generate NLP variable structure
	 */
	for( int k=0; k<N; k++ ) {

		dm_nlp["t"].append( DM({( k+tau(0) )*h}) );

		mx_nlp["x"].append( MX::sym( appOcp.y.name()+"_"+std::to_string(k), appOcp.y.sparsity() ) );
		dm_nlp["lbx"].append( appOcp.y.lbx() );
		dm_nlp["ubx"].append( appOcp.y.ubx() );
		dm_nlp["x0"].append( appOcp.y.x0() );

		for( int j=1; j<K+1; j++ ) {

			dm_nlp["t"].append( DM({( k+tau(j) )*h}) );

			mx_nlp["x"].append( MX::sym( appOcp.y.name()+"_"+std::to_string(k)+"_"+std::to_string(j), appOcp.y.sparsity() )  );
			dm_nlp["lbx"].append( appOcp.y.lbx() );
			dm_nlp["ubx"].append( appOcp.y.ubx() );
			dm_nlp["x0"].append( appOcp.y.x0() );
		}

		mx_nlp["x"].append( MX::sym( appOcp.u.name()+"_"+std::to_string(k), appOcp.u.sparsity() ) );
		dm_nlp["lbx"].append( appOcp.u.lbx() );
		dm_nlp["ubx"].append( appOcp.u.ubx() );
		dm_nlp["x0"].append( appOcp.u.x0() );
	}

	/*
	 * append terminal state x(N) = f(x(N-1), u(N-1)). consider collocation point
	 */
	dm_nlp["t"].append( DM({N*h}) );

	mx_nlp["x"].append( MX::sym( appOcp.y.name()+"_"+std::to_string(N), appOcp.y.sparsity() )  );
	dm_nlp["lbx"].append( appOcp.y.lbx() );
	dm_nlp["ubx"].append( appOcp.y.ubx() );
	dm_nlp["x0"].append( appOcp.y.x0() );

}

void NlpC::transcribe_nlp( void )
{

	// lambda functions for quick index referencing of DM/MX matrices
	auto y = [this](int k)       { return mx_nlp["x"]( k*stage_offset + y_offset ); };
	auto c = [this](int k, int j){ return mx_nlp["x"]( k*stage_offset + c_offset - 1 + j ); };
	auto u = [this](int k)       { return mx_nlp["x"]( k*stage_offset + u_offset ); };
	auto t = [this](int k, int j){ return dm_nlp["t"]( k*(1+K) +j ); };

	// transcribe the OCP problem
	for( int k=0; k<N; k++ ) {


		// continuity boundary constraints
		MX y_k_end = D(0)*y(k);

		for( int tau_j = 1; tau_j<K+1; tau_j++ )
			y_k_end += D(tau_j)*c( k, tau_j );

		mx_nlp["g"].append( y(k+1)-y_k_end );
		dm_nlp["lbg"].append( DM().zeros(appOcp.y.sparsity()) );
		dm_nlp["ubg"].append( DM().zeros(appOcp.y.sparsity()) );

		// residual constraints for collocation equations
		for( int tau_i=1; tau_i<K+1; tau_i++ ) {

			MX y_k_p = C(0,tau_i)*y(k);

			for( int tau_j=1; tau_j<K+1; tau_j++ )
				y_k_p += C(tau_j, tau_i)*c(k,tau_j);

			if( casadiFn["f"].exists() ) {
				mx_nlp["g"].append( y_k_p - h*casadiFn["f"](t(k,tau_i),c(k,tau_i),u(k)) );
				dm_nlp["lbg"].append( DM().zeros(appOcp.y.sparsity()) );
				dm_nlp["ubg"].append( DM().zeros(appOcp.y.sparsity()) );
			}

		}

		// add mpc stage cost / terminal cost
		std::string cost_type = k<N-1 || N == 1 ? "l" : "m";

		if( casadiFn[ cost_type ].exists() )
			for( int tau_i=1; tau_i<K+1; tau_i++)
				mx_nlp[ "J" ] += h*B( tau_i )*casadiFn[ cost_type ](t(k,tau_i),c(k,tau_i),u(k));

	}

	// structure to NLP solver
	const MXDict nlp = {{"x", mx_nlp["x"].concatenate()},
			            {"f", mx_nlp["J"].concatenate()},
				        {"g", mx_nlp["g"].concatenate()}};

	// instantiate NLP sovler
	solver = nlpsol("nlpsol", "ipopt", nlp, {{"verbose", false}} );

	DMDict arg = {{"x0",dm_nlp["x0"].concatenate()},
			{"lbx",dm_nlp["lbx"].concatenate()},
			{"ubx", dm_nlp["ubx"].concatenate()},
			{"lbg", dm_nlp["lbg"].concatenate()},
			{"ubg", dm_nlp["ubg"].concatenate()}};


	// arguments to NLP solver
	arg_nlp = {{"x0",dm_nlp["x0"].concatenate()},
		       {"lbx",dm_nlp["lbx"].concatenate()},
		       {"ubx", dm_nlp["ubx"].concatenate()},
		       {"lbg", dm_nlp["lbg"].concatenate()},
		       {"ubg", dm_nlp["ubg"].concatenate()}};
}





