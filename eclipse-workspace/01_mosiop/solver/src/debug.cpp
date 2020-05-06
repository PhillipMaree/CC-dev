/*
 * debug.cpp
 *
 *  Created on: Apr 27, 2020
 *      Author: johannes
 */
#include "../../solver/inc/debug.h"


#ifdef DEBUG_ENABLED

	void DEBUG( const int val, const std::string& str   )
	{
		printf("\033[1;30mDEBUG:\033[0m %s = %d\n", str.c_str(), val);
	}

	void DEBUG( const float val, const std::string& str   )
	{
		printf("\033[1;30mDEBUG:\033[0m %s = %10.3e\n", str.c_str() ,val);
	}

	void DEBUG( const std::string& str  )
	{
		printf("\033[1;30mDEBUG:\033[0m %s\n", str.c_str());
	}

	void DEBUG( const casadi::SX &sx, const std::string& str )
	{

	}

	void DEBUG( const casadi::DM &dm, const std::string& str )
	{
		if( !str.empty() )
			printf("\033[1;30mDEBUG:\033[0m %s[%dx%d] = \n", str.c_str() , (int)dm.size1(), (int)dm.size2());
		else
			printf("\033[1;30mDEBUG:\033[0m DM[%dx%d] = \n", (int)dm.size1(), (int)dm.size2());

		for( int i =0; i<dm.size1(); i++) {

			if( i==0 )
				printf("[[ ");
			else
				printf(" [ ");

			for ( int j=0; j<dm.size2(); j++ ) {
				if( i==dm.size1()-1 && j==dm.size2()-1)
					printf("%10.3e ]]\n", dm(i,j).operator double());
				else if( j==dm.size2()-1 )
					printf("%10.3e ],\n", dm(i,j).operator double());
				else
					printf("%10.3e, ", dm(i,j).operator double());
			}
		}
	}

	void DEBUG( const casadi::MX &mx, const std::string& str )
	{
		if( !str.empty() )
			printf("\033[1;30mDEBUG:\033[0m %s[%dx%d] = \n", str.c_str() , (int)mx.size1(), (int)mx.size2());
		else
			printf("\033[1;30mDEBUG:\033[0m MX[%dx%d] = \n", (int)mx.size1(), (int)mx.size2());

		for( int i =0; i<mx.size1(); i++) {

			if( i==0 )
				printf("[[ ");
			else
				printf(" [ ");

			for ( int j=0; j<mx.size2(); j++ ) {
				if( i==mx.size1()-1 && j==mx.size2()-1)
					printf("%s ]]\n", mx(i,j).get_str().c_str() );
				else if( j==mx.size2()-1 )
					printf("%s ]\n", mx(i,j).get_str().c_str());
				else
					printf("%s, ", mx(i,j).get_str().c_str());
			}
		}
	}

	void DEBUG( const std::vector<casadi::MX> &v, const std::string& str )
	{
		if( !str.empty() )
			printf("\033[1;30mDEBUG:\033[0m %s[1x%d] = ", str.c_str() , (int)v.size() );
		else
			printf("\033[1;30mDEBUG:\033[0m Vec<MX>[1x%d] = ", (int)v.size() );


		for( int i =0; i<v.size(); i++) {
			if( i==0 )
				printf("[[ %10.3e, ", v[i].operator double() );
			else if( i == v.size()-1 )
				printf("%10.3e ]]", v[i].operator double() );
			else
				printf("%10.3e, ", v[i].operator double() );
		}
	}

	void DEBUG( const std::vector<casadi::DM> &v, const std::string& str )
		{
		if( !str.empty() )
			printf("\033[1;30mDEBUG:\033[0m %s[1x%d] = ", str.c_str() , (int)v.size() );
		else
			printf("\033[1;30mDEBUG:\033[0m Vec<DM>[1x%d] = ", (int)v.size() );


		for( int i =0; i<v.size(); i++) {
			if( i==0 )
				printf("[[ %10.3e, ", v[i].operator double() );
			else if( i == v.size()-1 )
				printf("%10.3e ]]", v[i].operator double() );
			else
				printf("%10.3e, ", v[i].operator double() );
		}
		}

#endif

