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

};

template <typename T>
std::vector<T> DictC<T>::ProxySt::concatenate( void )
{

	std::vector<T> v_;
	typename std::vector<T>::iterator itr;

	for( itr = v.begin(); itr != v.end(); itr++ )
		for( int i=0; i<itr.operator *().size1() ; i++ )
			v_.push_back( itr.operator *()(i) );

	return v_;
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
		casadi::Function l = casadi::Function( "l", { appOcp->t, appOcp->y, appOcp->u }, {appOcp->l()}, {"t", "y", "u"}, {"l"} ) ;
		fn_map.insert( std::pair<std::string, casadi::Function>("l", l ));
	}

	if( appOcp->m().sparsity().density() ) {
		casadi::Function m = casadi::Function( "fn_M", { appOcp->t, appOcp->y, appOcp->u }, {appOcp->m()}, {"t", "y", "u"}, {"m"} ) ;
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

NlpC::NlpC( float tf_, int N_ ) :
		ColC( 3, "legendre"), appOcp( tf_ ), casadiFn(&appOcp),
		tf(tf_), h(tf_/N), N(N_),
		n(appOcp.y.size1()), m(appOcp.u.size1()),
		y_offset(0), c_offset(1), u_offset(1+K), stage_offset(2+K)
{
	build_nlp_struct();
	transcribe_nlp();

	report();
}

void NlpC::report( void )
{
	int nlp_u_var_n = N*m;
	int nlp_y_var_n = (N+1)*n;
	int nlp_c_var_n = N*K*n;
	int nlp_t_var_n = nlp_u_var_n+nlp_y_var_n+nlp_c_var_n;

	printf( "MPC problem for application <%s> with OCP final time horizon tf=%.1f\n"
			"with prediction horizon N=%d at stage step-size h=%.1f\n\n", appOcp.name.c_str(), tf,  N, h );

	printf("NLP initialized with collocation scheme: %s of degree(order) %d(%d)\n", scheme.c_str(), K, K+1 );

	printf("with: %d total variables of which:\n"
		   "      %d differential variables\n"
		   "      %d piece-wise constant control variables\n"
		   "      %d collocation variables\n", nlp_t_var_n, nlp_y_var_n, nlp_u_var_n, nlp_c_var_n);

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
	dm_nlp["t"].append( DM({N*h}) );

	mx_nlp["x"].append( MX::sym( appOcp.y.name()+"_"+std::to_string(N), appOcp.y.sparsity() )  );
	dm_nlp["lbx"].append( appOcp.y.lbx() );
	dm_nlp["ubx"].append( appOcp.y.ubx() );
	dm_nlp["x0"].append( appOcp.y.x0() );

}

void NlpC::transcribe_nlp( void )
{
	/*
	 * lambda functions for quick index referencing of DM/MX matrices
	 */
	auto y = [this](int k)       { return mx_nlp["x"]( k*stage_offset + y_offset ); };
	auto c = [this](int k, int j){ return mx_nlp["x"]( k*stage_offset + c_offset - 1 + j ); };
	auto u = [this](int k)       { return mx_nlp["x"]( k*stage_offset + u_offset ); };
	auto t = [this](int k, int j){ return dm_nlp["t"]( k*(1+K) +j ); };

	/*
	 * transcribe the OCP problem
	 */


	if( casadiFn["f"].exists() )
	{
		DEBUG("Function exists");
		DEBUG(casadiFn["f"](t(1,1),c(1,1),u(0)));
	}
	else{
		DEBUG("Function does not exists");
	}

	//MXDict f_tau_i =fn_map.find( "fn_f" )->second( MXDict{{"t",t(1,1)},{"y",c(1,1)},{"u",u(0)}} );

	//std::cout << y_k_p-f_tau_i.find("f")->second*h;

	//std::cout << y_k_p << "\n";
	//std::cout << f_tau_i.<< "\n";
	//std::cout << f_tau_i[1]*h << "\n";



#if 0
	for( int k=0; k<N; k++ ) {

		/*
		 * continuity boundary constraints
		 */
		MX y_k_end = D(0)*y(k);

		for( int tau_j = 1; tau_j<K+1; tau_j++ )
			y_k_end += D(tau_j)*c( k, tau_j );

		mx_nlp["g"].append( y(k+1)-y_k_end );
		dm_nlp["lbg"].append( DM().zeros(appOcp.y.sparsity()) );
		dm_nlp["ubg"].append( DM().zeros(appOcp.y.sparsity()) );

		/*
		 * residual constraints for collocation equations
		 */
		for( int tau_i=1; tau_i<K+1; tau_i++ ) {

			MX y_k_p = C(0,tau_i)*y(k);

			for( int tau_j=1; tau_j<K+1; tau_j++ )
				y_k_p += C(tau_j, tau_i)*c(k,tau_j);

			if( linkedFn( "fn_f", fn ) ) {

				std::vector<MX> f_tau_i =fn( std::vector<casadi::MX>({t(k,tau_i),c(k,tau_i),u(k)}) );


				//mx_nlp["g"].append( y_k_p(1) - f_tau_i(1) );
				//dm_nlp["lbg"].append( DM().zeros(appOcp.y.sparsity()) );
				//dm_nlp["ubg"].append( DM().zeros(appOcp.y.sparsity()) );

			}

		}


		/*
		 *
		 */
	}

#endif



}




