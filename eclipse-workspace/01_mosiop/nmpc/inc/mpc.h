/*
 * solver.h
 *
 *  Created on: Apr 23, 2020
 *      Author: johannes
 */

#ifndef INC_MPC_H_
#define INC_MPC_H_

#include "../inc/nlp.h"

class MpcC {
public:
	MpcC( float tf, int N ):nlp(tf, N){};
	~MpcC( void);

protected:

	NlpC nlp;


};




#endif /* INC_MPC_H_ */
