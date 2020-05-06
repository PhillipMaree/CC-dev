/*
 * nlp.h
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */

#ifndef INCLUDE_NLP_H_
#define INCLUDE_NLP_H_

#include <iterator>
#include "../../solver/inc/ocp.h"

#ifdef __cplusplus
extern "C" {
#endif

void* create_solver( double tf, double N );
void destroy_solver( void* vptr );
double solve(void * vptr, double x1, double x2, double x3 );

#ifdef __cplusplus
}
#endif

class ColC {
public:

	ColC( int degree, std::string scheme );
	~ColC( void ){};

protected:

	casadi::DM B, C, D, tau;
	std::string scheme;
	int K;



};

template <typename T>
class DictC {
public:

	DictC( void ) {}
	~DictC( void ) {}

protected:

	struct ProxySt {
	public:

		ProxySt(  std::vector<T>& T_vec ) : vec(T_vec) {}

        void append( T val) { vec.push_back( val ); }
        T concatenate( void ) { return vertcat(vec); };

        T& operator()(int k) { return vec[k]; }
        void operator+=(T val);

	protected:
		std::vector<T>& vec;
	};

	typename std::map<std::string, std::vector<T>> T_dict;
    typename std::map<std::string, std::vector<T>>::iterator itr;

public:

    ProxySt operator[](std::string key);

};

class CasadiFnC {

public:

	CasadiFnC( AppOcpC* appOcp_ ) : appOcp(appOcp_) {
		link_fn( );
	}
	~CasadiFnC( void ) {}

protected:

	AppOcpC* appOcp;

	struct ProxySt {
	public:
		ProxySt( void ) : fn_exists(false) { };
		ProxySt(  std::string fn_n, casadi::Function fn_h  ) : fn_exists(true), fn_name(fn_n), fn_handle(fn_h) {}

		bool exists() { return fn_exists; }

		casadi::MX operator()(casadi::MX t, casadi::MX y, casadi::MX u) {
			return fn_handle( casadi::MXDict( {{"t",t},{"y", y},{"u",u}} ) ).find(fn_name)->second  ;
		}

	protected:
		bool fn_exists;
		casadi::Function fn_handle;
		std::string fn_name;
	};


	std::map<std::string, casadi::Function> fn_map;
	std::map<std::string, casadi::Function>::iterator itr;

	void link_fn( void );

public:

	ProxySt operator[](std::string func_name);

};

class NlpC : public ColC {
public:

	NlpC( float , int, int K=3, std::string scheme="legendre");
	~NlpC( void ){};

	casadi::DMDict solve(casadi::DMDict&);

protected:

	AppOcpC appOcp;
	CasadiFnC casadiFn;
	casadi::Function solver;

	bool linked_fn( std::string fn, casadi::Function& link );
	void build_nlp_struct( void );
	void transcribe_nlp( void );
	void report( void );

	DictC<casadi::MX> mx_nlp;
	DictC<casadi::DM> dm_nlp;
	casadi::DMDict arg_nlp;

	const float tf, h;
	const int N, n, m;
	const int y_offset, c_offset, u_offset, stage_offset, nlp_u_var_n, nlp_y_var_n, nlp_c_var_n, nlp_g_var_n, nlp_t_var_n;

};

#endif /* INCLUDE_NLP_H_ */
