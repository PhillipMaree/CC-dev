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

	void DEBUG( const int val, const std::string& str="Val" );
	void DEBUG( const float val, const std::string& str="Val" );
	void DEBUG( const std::string& str  );
	void DEBUG( const boost::format& fmt );
	void DEBUG( const casadi::SX &sx, const std::string& str="" );
	void DEBUG( const casadi::DM &dm, const std::string& str="" );
	void DEBUG( const casadi::MX &mx, const std::string& str="" );
	void DEBUG( const std::vector<casadi::DM> &v, const std::string& str="" );
	void DEBUG( const std::vector<casadi::MX> &v, const std::string& str="" );

#else

	inline void DEBUG( const int val, const std::string& str="Val" ) {};
	inline void DEBUG( const float val, const std::string& str="Val" ) {};
	inline void DEBUG( const std::string& str  ){};
	inline void DEBUG( const boost::format& fmt ){};
	inline void DEBUG( const casadi::SX &sx, const std::string& str="" ){};
	inline void DEBUG( const casadi::DM &sx, const std::string& str="" ){};
	inline void DEBUG( const casadi::MX &sx, const std::string& str="" ){};
	inline void DEBUG( const std::vector<casadi::DM> &v, const std::string& str="" ){};
	inline void DEBUG( const std::vector<casadi::MX> &v, const std::string& str="" ){};

#endif

#endif /* DEBUG_DEBUG_H_ */
