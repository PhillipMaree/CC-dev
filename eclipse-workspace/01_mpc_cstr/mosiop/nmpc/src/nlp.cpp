/*
 * nlp.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../inc/nlp.h"

/*
 * NLP formulation class functions
 */

NlpC::NlpC(  float tf_, int N_ ):ocp(tf), tf(tf_), N(N_), dt(tf_/N)
{

};

/*
 * Collocation class functions
 */

CollocationC::CollocationC(int degree, std::string method):K(degree)
{
	int order = K+1;

	tau_root.VectorC({0,casadi::collocation_points(degree, method)});
	B.VectorC(order);
	D.VectorC(order);
	C.MatrixC(order,order);

	printf("Implement a %s collocation method of order %d with root coefficients:\n", method, order);

};
