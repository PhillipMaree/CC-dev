/*
 * nlp.h
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */

#ifndef INC_NLP_H_
#define INC_NLP_H_



#include "../inc/ocp.h"

class NlpC {
public:
	NlpC( float, int );
	~NlpC( void ){};

protected:

	TargetOcpC ocp;

	float tf, dt;
	int N;

};

class CollocationC {
public:

	CollocationC(int degree, std::string method) : K(degree),
		tau_root(casadi::DM({0,casadi::collocation_points(degree, method)})) ,
		B(casadi::DM(1, K+1)), C(casadi::DM(K+1, K+1)), D(casadi::DM(1, K+1)) {};
	~CollocationC( void ){};

protected:

	int K;
	casadi::DM B, C, D, tau_root;

};



#endif /* INC_NLP_H_ */
