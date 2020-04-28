/*
 * nlp.h
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */

#ifndef INCLUDE_NLP_H_
#define INCLUDE_NLP_H_

#include "../inc/ocp.h"

class ColC {
public:

	ColC( int, std::string);
	~ColC( void ){};

protected:

	int K;
	casadi::DM B, C, D, tau_root;


};

class NlpC : public ColC {
public:

	NlpC( float tf, int N_ ) : ColC( 4, "legendre"), ocp( tf ), h(tf/N_), N(N_) {}
	~NlpC( void ){};

protected:

	AppOcpC ocp;
	float h;
	int N;

};

#endif /* INCLUDE_NLP_H_ */
