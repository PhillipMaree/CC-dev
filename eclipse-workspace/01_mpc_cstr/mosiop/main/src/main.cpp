/*
 * main.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */
#include "../../nmpc/inc/math.h"
#include "debug.h"

void TEST_PRINT_STRING( void )
{
	DEBUG("MPC testing commence.");
}

void TEST_PRINT_VECTOR( void )
{
	VectorC<float> v({1,2,3,4,5,6,7,8,9,0});

	DEBUG(v, "v Vector Print");

}

void TEST_PRINT_MATRIX( void )
{
	MatrixC<float> m(5,6);

	for(int i=0; i<m.rows(); i++)
		for(int j=0; j<m.cols(); j++)
			m[i][j] = i+j;


	DEBUG(m, "Matrix Print of size [5,6]");

}

int main( void )
{
	TEST_PRINT_STRING();
	TEST_PRINT_VECTOR();
	TEST_PRINT_MATRIX();

	return 0;
}



