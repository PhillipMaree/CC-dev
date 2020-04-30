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

	void append(std::string key, T val) {
		std::vector<T> v(val);

		itr = T_dict.find( key );
		if( itr == T_dict.end() )
			T_dict[key].insert(T_dict[key].end(),v.begin() , v.end());
		else
			itr->second.insert(itr->second.end(),v.begin() , v.end());
	}

	T eval(std::string key) {

		itr = T_dict.find( key );
		if( itr != T_dict.end() )
			return T(itr->second);
		else
			return T();

	}

	struct ProxySt {
	public:
		ProxySt(  std::vector<T>& v_ ) : v(v_) {}


		void append( T val) {
			std::vector<T> v_(val);
			v.insert(v.end(),v_.begin() , v_.end());
		}

		T eval( void ) {
			return T(v);
		}
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

	AppOcpC ocp;
	float h;
	int N;
	int n,m;

	DictC<casadi::MX> nlp;
	DictC<casadi::DM> arg;

	VarStatsSt stats;
	IdxOffSt offsets;

	void create( void );
	void transcribe( void );

	void append(const double&);
	void append(const casadi::MX&);
	void append(const casadi::MX&, const casadi::DM&, const casadi::DM&);
	void append(const casadi::MX&, const casadi::DM&, const casadi::DM&, const casadi::DM&);


};

#endif /* INCLUDE_NLP_H_ */
