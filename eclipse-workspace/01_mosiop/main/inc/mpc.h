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

	MpcC( float h_, int N, int T_, int K=3) : h(h_), T(T_), nlp( h_, N, K ){ };
	~MpcC( void){};

	casadi::DMDict solve(casadi::DMDict&, bool closed_loop=true );

protected:

	NlpC nlp;
	const int T;
	const float h;

};

#endif /* INC_MPC_H_ */
