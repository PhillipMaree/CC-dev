/*
 * debug.h
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */

#ifndef DEBUG_DEBUG_H_
#define DEBUG_DEBUG_H_

#include <casadi/casadi.hpp>
#include <boost/format.hpp>
#include <string>
#include <vector>

#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED

	void DEBUG( const std::string& str  )
	{
		printf("%s\n", str.c_str());
	}

	void DEBUG( const casadi::SX &sx, const std::string& str="" )
	{

	}

	void DEBUG( const casadi::DM &dm, const std::string& str="" )
	{
		if( !str.empty() )
			std::cout << str << "\n";

		std::cout << dm << "\n";
	}

	void DEBUG( const casadi::MX &mx, const std::string& str="" )
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

#else

	inline void DEBUG( const std::string str ){};
	inline void DEBUG( const casadi::SX &sx ){};
	inline void DEBUG( const casadi::DX &sx ){};
	inline void DEBUG( const casadi::MX &sx ){};

#endif

#endif /* DEBUG_DEBUG_H_ */
