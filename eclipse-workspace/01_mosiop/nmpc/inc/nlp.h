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

	ColC( int degree=3, std::string scheme="legendre" );
	~ColC( void ){};

protected:

	int K;
	std::string scheme;
	casadi::DM B, C, D, tau;

};

template <typename T>
class DictC {
public:

	DictC( void ) {}
	~DictC( void ) {}

	struct ProxySt {
	public:
		ProxySt(  std::vector<T>& v_ ) : v(v_) {}


		void append( T val) {
			for (int i=0; i< val.size1(); i++)
				v.push_back(val(i));
		}

		std::vector<T> eval( void ) { return v; }

	protected:
		std::vector<T>& v;
	};

	ProxySt operator[](std::string key) {
		itr = T_dict.find( key );
		if( itr != T_dict.end() )
			return ProxySt( itr->second ) ;
		else
			return ProxySt( T_dict[key] );
	}

protected:

	typename std::map<std::string, std::vector<T>> T_dict;
    typename std::map<std::string, std::vector<T>>::iterator itr;

};

class NlpC : public ColC {
public:

	NlpC( float tf, int N_);
	~NlpC( void ){};

	void report( void );

protected:

	// NLP class variable statistics
	struct VarStatsSt {
	public:

		VarStatsSt(const int n, const int m, const int N, const int K) {
			nlp_u_var_n = N*m;
			nlp_y_var_n = (N+1)*n;
			nlp_c_var_n = N*K*n;
			nlp_t_var_n = nlp_u_var_n+nlp_y_var_n+nlp_c_var_n;
		}
		~VarStatsSt(void){}

		void report( void ) {
			printf("NLP problem created with: %d total variables of which:\n"
			       "                          %d differential variables\n"
			       "                          %d piece-wise constant control variables\n"
			       "                          %d collocation variables\n", nlp_t_var_n, nlp_y_var_n, nlp_u_var_n, nlp_c_var_n);
		}
	private:
		int nlp_u_var_n, nlp_y_var_n, nlp_c_var_n, nlp_t_var_n;
	};

	// NLP fast index offset
	struct IdxOffSt {
	public:
		IdxOffSt(const int K) {
			yoff=0;
			coff=yoff+1;
			uoff=coff+K;
			poff=poff+1;
		}
		~IdxOffSt( void ){}

	private:
		int yoff, coff, uoff, poff;
	};

	AppOcpC ocp;       // application spesific OCP
	float h;           // time duration of MPC stage
	int N;             // prediction horizon number of stages
	int n,m;           // state and control dimension size

	DictC<casadi::MX> mx_nlp;
	DictC<casadi::DM> dm_nlp;

	VarStatsSt stats;
	IdxOffSt offsets;

	void structure( void );
	void transcribe( void );
};

#endif /* INCLUDE_NLP_H_ */
