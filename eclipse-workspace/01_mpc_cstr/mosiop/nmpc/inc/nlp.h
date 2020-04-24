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

	CollocationC(int degree, std::string method);
	~CollocationC( void ){};

protected:

	int K;
	VectorC<float> B, D, tau_root;
	MatrixC<float> C;

};



#endif /* INC_NLP_H_ */
