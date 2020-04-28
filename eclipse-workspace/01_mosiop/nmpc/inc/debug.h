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

	void DEBUG( const std::string& str  );
	void DEBUG( const boost::format& fmt );
	void DEBUG( const casadi::SX &sx, const std::string& str="" );
	void DEBUG( const casadi::DM &dm, const std::string& str="" );
	void DEBUG( const casadi::MX &mx, const std::string& str="" );

#else

	inline void DEBUG( const std::string& str  ){};
	inline void DEBUG( const boost::format& fmt ){};
	inline void DEBUG( const casadi::SX &sx ){};
	inline void DEBUG( const casadi::DM &sx ){};
	inline void DEBUG( const casadi::MX &sx ){};

#endif

#endif /* DEBUG_DEBUG_H_ */