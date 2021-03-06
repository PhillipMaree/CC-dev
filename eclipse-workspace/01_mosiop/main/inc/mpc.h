/*
 * solver.h
 *
 *  Created on: Apr 23, 2020
 *      Author: johannes
 */

#ifndef INC_MPC_H_
#define INC_MPC_H_

#include <string>
#include "../../solver/inc/nlp.h"

class MpcC {
public:

	MpcC( float h_, int N, int tf_, int K=3) : h(h_), tf(tf_), nlp( h_, N, K ){ };
	~MpcC( void){};

	casadi::DMDict solve(casadi::DMDict&, bool closed_loop=true );

protected:

	NlpC nlp;
	const int tf;
	const float h;

};

#endif /* INC_MPC_H_ */
