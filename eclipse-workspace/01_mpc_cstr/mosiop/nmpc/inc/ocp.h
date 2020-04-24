/*
 * ocp.h
 *
 *  Created on: Apr 23, 2020
 *      Author: johannes
 */
#ifndef INCLUDE_OCP_H_
#define INCLUDE_OCP_H_

#include <casadi/casadi.hpp>
#include "../inc/math.h"

/*
 * Class for handling complex Casadi variables types
 */
template<class T>
class VarC {

public:

	VarC( void ){};
	VarC( std::string name_, std::initializer_list<T> val_ ){
		init(  name_, val_, val_ );
	};
	VarC( std::string name_, std::initializer_list<T> lb_, std::initializer_list<T> ub_ ){
		init(  name_, lb_, ub_ );
	};
	~VarC( void ){};

	void init(  std::string, std::initializer_list<T>, std::initializer_list<T> );

	std::string name;
	VectorC<T> ub;
	VectorC<T> lb;
	casadi::MX v;

};

/*
 * OCP formulation based on terminology of Betts, 2010 Ch4, p 123
 */
class OcpC {

public:



	OcpC( float );
	virtual ~OcpC( void );

	virtual casadi::MX L(void){ return casadi::MX(); }  // legendre cost term
	virtual casadi::MX M(void){ return casadi::MX(); }  // mayer cost term

	virtual casadi::MX f(void){ return casadi::MX(); }  // dynamic state equality constraints
	virtual casadi::MX g(void){ return casadi::MX(); }  // algebraic equality constraints
	virtual casadi::MX h(void){ return casadi::MX(); }  // algebraic inequality constraints

protected:

	std::string name;                                  // name description of problem
	VarC<float> t,y,u,p;                                      // time, states, control, parameter variables
};

/*
 * Application class
 */
class CstrC: public OcpC {
public:

	CstrC( float );

	casadi::MX L(void){ return casadi::MX(); }
	casadi::MX M(void){ return casadi::MX(); }

	casadi::MX f(void){ return casadi::MX(); }
	casadi::MX g(void){ return casadi::MX(); }
	casadi::MX h(void){ return casadi::MX(); }

};

/*
 * Application target to by transcribed
 */
typedef CstrC TargetOcpC;


#endif /* INCLUDE_OCP_H_ */
