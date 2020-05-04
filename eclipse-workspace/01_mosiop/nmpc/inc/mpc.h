/*
 * solver.h
 *
 *  Created on: Apr 23, 2020
 *      Author: johannes
 */

#ifndef INC_MPC_H_
#define INC_MPC_H_

#include "../inc/nlp.h"
#include <string>

class MpcC {
public:

	MpcC( float tf, int N, int K=3) : nlp( tf, N, K ){ };
	~MpcC( void){};

protected:

	NlpC nlp;

};




#endif /* INC_MPC_H_ */
