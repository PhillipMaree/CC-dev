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

	MpcC( float h, int N, int K=3) : nlp( h, N, K ){ };
	~MpcC( void){};

	void solve(casadi::DMDict&, casadi::DMDict&);

protected:

	NlpC nlp;

};




#endif /* INC_MPC_H_ */
