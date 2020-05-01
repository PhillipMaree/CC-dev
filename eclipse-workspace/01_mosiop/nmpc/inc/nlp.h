/*
 * nlp.h
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */

#ifndef INCLUDE_NLP_H_
#define INCLUDE_NLP_H_

#include "../inc/ocp.h"
#include <iterator>

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
		ProxySt(  std::vector<T>& v_ ) : v(v_) {}

        void append( T val) { v.push_back( val ); }
        std::vector<T> concatenate( void );
        T& operator()(int k) { return v[k]; }

	protected:
		std::vector<T>& v;
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

	NlpC( float tf, int N_);
	~NlpC( void ){};

protected:


	AppOcpC appOcp;
	CasadiFnC casadiFn;

	bool linked_fn( std::string fn, casadi::Function& link );
	void build_nlp_struct( void );
	void transcribe_nlp( void );
	void report( void );

	DictC<casadi::MX> mx_nlp;
	DictC<casadi::DM> dm_nlp;

	const float tf, h;
	const int N, n, m;
	const int y_offset, c_offset, u_offset, stage_offset;

};

#endif /* INCLUDE_NLP_H_ */
