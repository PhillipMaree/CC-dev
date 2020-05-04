/*
 * debug.cpp
 *
 *  Created on: Apr 27, 2020
 *      Author: johannes
 */
#include "../inc/debug.h"


#ifdef DEBUG_ENABLED

	void DEBUG( const int val, const std::string& str   )
	{
		printf("%s = %d\n", str.c_str(), val);
	}

	void DEBUG( const float val, const std::string& str   )
	{
		printf("%s = %.3f\n", str.c_str() ,val);
	}

	void DEBUG( const std::string& str  )
	{
		printf("DEBUG: %s\n", str.c_str());
	}

	void DEBUG( const boost::format& fmt )
	{
		printf("DEBUG: %s\n", boost::str(fmt ).c_str() );
	}

	void DEBUG( const casadi::SX &sx, const std::string& str )
	{

	}

	void DEBUG( const casadi::DM &dm, const std::string& str )
	{
		if( !str.empty() )
			std::cout << "DEBUG: " << str << boost::format{"[%1%x%2%] = "} % dm.size1() % dm.size2();
		else
			std::cout << "DEBUG: DX"      << boost::format{"[%1%x%2%] = "} % dm.size1() % dm.size2();


		for( int i =0; i<dm.size1(); i++) {

			if( i==0 )
				std::cout << "[[ ";
			else
				std::cout << "DEBUG:  [ ";

			for ( int j=0; j<dm.size2(); j++ ) {
				if( i==dm.size1()-1 && j==dm.size2()-1)
					std::cout << dm(i,j) << " ]]\n";
				else if( j==dm.size2()-1 )
					std::cout << dm(i,j) << " ]\n";
				else
					std::cout << dm(i,j) << ", ";
			}
		}
	}

	void DEBUG( const casadi::MX &mx, const std::string& str )
	{
		if( !str.empty() )
			std::cout << "DEBUG: " << str << boost::format{"[%1%x%2%] = "} % mx.size1() % mx.size2();
		else
			std::cout << "DEBUG: MX"      << boost::format{"[%1%x%2%] = "} % mx.size1() % mx.size2();

		for( int i =0; i<mx.size1(); i++) {

			if( i==0 )
				std::cout << "[[ ";
			else
				std::cout << "DEBUG:  [ ";

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

	void DEBUG( const std::vector<casadi::MX> &v, const std::string& str )
	{
		if( !str.empty() )
			std::cout << "DEBUG: " << str << boost::format{"[1x%1%] = "} %  v.size();
		else
			std::cout << "DEBUG: MX"      << boost::format{"[1x%1%] = "} %  v.size();


		for( int i =0; i<v.size(); i++) {
			if( i==0 )
				std::cout << "[[ "<< v[i] << ", ";
			else if( i == v.size()-1 )
				std::cout << v[i] << " ]]\n";
			else
				std::cout << v[i] << ", ";
		}
	}

	void DEBUG( const std::vector<casadi::DM> &v, const std::string& str )
		{
			if( !str.empty() )
				std::cout << "DEBUG: " << str << boost::format{"[1x%1%] = "} %  v.size();
			else
				std::cout << "DEBUG: DM"      << boost::format{"[1x%1%] = "} %  v.size();


			for( int i =0; i<v.size(); i++) {
				if( i==0 )
					std::cout << "[[ "<< v[i] << ", ";
				else if( i == v.size()-1 )
					std::cout << v[i] << " ]]\n";
				else
					std::cout << v[i] << ", ";
			}
		}

#endif

