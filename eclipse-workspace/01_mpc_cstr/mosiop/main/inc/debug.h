/*
 * debug.h
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */

#ifndef DEBUG_DEBUG_H_
#define DEBUG_DEBUG_H_


#include <string>
#include <vector>

#define DEBUG_ENABLED


#ifdef DEBUG_ENABLED

	void DEBUG( const std::string str );
	template<typename T>
	void DEBUG(const std::vector<T> &vector,const std::string str)
	{

		if( !str.empty() )
			printf("%s\n", str.c_str());

		const int size = vector.size();

		for(int i=0; i<size; i++){
			if(i==0)
				printf("[%.3e,", vector[i]);
			else if(i==size-1)
				printf(" %.3e]\n", vector[i]);
			else
				printf(" %.3e,", vector[i]);
		}
	}

	template<typename T>
	void DEBUG(const std::vector<std::vector<T>> &matrix,const std::string str)
	{
		if( !str.empty() )
			printf("%s\n", str.c_str());

		const int rows = matrix.size();
		const int cols = matrix[0].size();

		for(int row=0; row<rows; row++){
			for(int col=0; col<cols; col++){
				if(row==0 && col==0 && cols==1)
					printf("[%.3e;\n", matrix[row][col]);
				else if(row==0 && col==0 )
					printf("[%.3e", matrix[row][col]);
				else if(row==rows-1 && col==cols-1)
					printf(" %.3e]\n", matrix[row][col]);
				else if(col==cols-1)
					printf(" %.3e;\n", matrix[row][col]);
				else
					printf(" %.3e,", matrix[row][col]);
			}
		}
	}

#else

	inline void DEBUG( const std::string str ){};
	template<typename T>
	inline void DEBUG( const std::vector<T> &,const std::string str=""){};             // prfloat float arrays
	template<typename T>
	inline void DEBUG( const std::vector<std::vector<T>> &,const std::string str=""){};

#endif

#endif /* DEBUG_DEBUG_H_ */
