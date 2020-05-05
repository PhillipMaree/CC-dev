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

#ifdef __cplusplus
extern "C" {
#endif

void* create_solver( double tf, double N, double K );
void destroy_solver( void* vptr );
double solve(void * vptr, double x1, double x2, double x3 );

#ifdef __cplusplus
}
#endif

class MpcC {
public:

	MpcC( float tf, int N, int K=3) : nlp( tf, N, K ){ };
	~MpcC( void){};

	void solve(casadi::DMDict&, casadi::DMDict&);

protected:

	NlpC nlp;

};




#endif /* INC_MPC_H_ */
