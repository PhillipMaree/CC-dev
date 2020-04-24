/*
 * solver.h
 *
 *  Created on: Apr 23, 2020
 *      Author: johannes
 */

#ifndef INC_MPC_H_
#define INC_MPC_H_

#include "../inc/nlp.h"

/*
 * Output structure
 */
struct MpcOSt {
	double u;
};

/*
 * Structure to contain desirable inputs needed to solve MPC
 */
struct MpcISt {
	double x[3];
};

class MpcC {
public:
	MpcC( float tf, int N ):nlp(tf, N){};
	~MpcC( void);

protected:

	NlpC nlp;


};




#endif /* INC_MPC_H_ */
