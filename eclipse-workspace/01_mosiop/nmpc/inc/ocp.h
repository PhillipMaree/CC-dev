/*
 * ocp.h
 *
 *  Created on: Apr 23, 2020
 *      Author: johannes
 */
#ifndef INCLUDE_OCP_H_
#define INCLUDE_OCP_H_

#include <casadi/casadi.hpp>
#include "../inc/debug.h"

/*
 * Class for handling complex Casadi variables types
 */
class VmC : public casadi::MX {

public:

	VmC( void ){};
	VmC( const std::string name, const casadi::DM& val ) : casadi::MX( casadi::MX::sym(name, val.size1(), val.size2()) ), vm_ub(val) {
		DEBUG( boost::format{"Added %1% [%2%x%3%]"} % name % this->size1() % this->size2() );
	}
	VmC( const std::string name, const casadi::DM& lb, const casadi::DM& ub ) : casadi::MX( casadi::MX::sym(name, lb.size1(), lb.size2()) ), vm_lb(lb), vm_ub(ub) {
		DEBUG( boost::format{"Added %1% [%2%x%3%]"} % name % this->size1() % this->size2() );
	}
	~VmC( void ){};

	const std::string name() { return this->get_str(); }
	const casadi::DM ub() { return vm_ub; };
	const casadi::DM lb() { return vm_lb.size1()==0 ? vm_ub : vm_lb; };

protected:

	casadi::DM vm_ub;
	casadi::DM vm_lb;

};
typedef VmC VM;

/*
 * OCP formulation based on terminology of Betts, 2010 Ch4, p 123
 */
class OcpC {

public:



	OcpC( float );
	virtual ~OcpC( void ) {};

	virtual casadi::MX L(void){ return casadi::MX(); }  // legendre cost term
	virtual casadi::MX M(void){ return casadi::MX(); }  // mayer cost term

	virtual casadi::MX f(void){ return casadi::MX(); }  // dynamic state equality constraints
	virtual casadi::MX g(void){ return casadi::MX(); }  // algebraic equality constraints
	virtual casadi::MX h(void){ return casadi::MX(); }  // algebraic inequality constraints

protected:

	std::string name;                                  // name description of problem
	VM t,y,u,p;                                      // time, states, control, parameter variables
};

/*
 * Application class
 */
class CstrC: public OcpC {
public:

	CstrC( float );

	casadi::MX L(void);
	casadi::MX M(void);

	casadi::MX f(void);

};

/*
 * Application target to by transcribed
 */
typedef CstrC AppOcpC;


#endif /* INCLUDE_OCP_H_ */
