/*
 * debug.cpp
 *
 *  Created on: Apr 27, 2020
 *      Author: johannes
 */
#include "../inc/debug.h"


#ifdef DEBUG_ENABLED

	void DEBUG( const std::string& str  )
	{
		printf("%s\n", str.c_str());
	}

	void DEBUG( const boost::format& fmt )
	{
		printf("%s\n", boost::str(fmt ).c_str() );
	}

	void DEBUG( const casadi::SX &sx, const std::string& str )
	{

	}

	void DEBUG( const casadi::DM &dm, const std::string& str )
	{
		if( !str.empty() )
			std::cout << str << "\n";

		std::cout << dm << "\n";
	}

	void DEBUG( const casadi::MX &mx, const std::string& str )
	{
		if( !str.empty() )
			std::cout << str << "\n";

		std::cout << boost::format{"[%1%x%2%]"} % mx.size1() % mx.size2()  << '\n';


		for( int i =0; i<mx.size1(); i++) {
			if( i==0 )
				std::cout << "[[ ";
			else
				std::cout << " [ ";
			for ( int j=0; j<mx.size2(); j++ ) {
				if( i==mx.size1()-1 && j==mx.size2()-1)
					std::cout << mx(i,j) << " ]]\n";
				else if( j==mx.size2()-1 )
					std::cout << mx(i,j) << " ]\n";
				else
					std::cout << mx(i,j) << ", ";
			}
		}
	}

#endif

