/*
 * ocp.h
 *
 *  Created on: Apr 23, 2020
 *      Author: johannes
 */
#ifndef INCLUDE_OCP_H_
#define INCLUDE_OCP_H_

#include <casadi/casadi.hpp>

#include "../../solver/inc/debug.h"

/*
 * Class for handling complex Casadi variables types
 */
class VmC : public casadi::MX {

public:

	VmC( void ){};
	VmC( const std::string name, const casadi::DM& val ) : casadi::MX( casadi::MX::sym(name, val.size1(), val.size2()) ), lbx_dm(val) {}
	VmC( const std::string name, const casadi::DM& lb, const casadi::DM& ub ) : casadi::MX( casadi::MX::sym(name, lb.size1(), lb.size2()) ), lbx_dm(lb), ubx_dm(ub) {}
	~VmC( void ){};

	const std::string name() { return this->get_str(); }
	const casadi::DM x0()  { return lbx_dm; }
	const casadi::DM lbx() { return lbx_dm; }
	const casadi::DM ubx() { return ubx_dm.size1()==0 ? lbx_dm : ubx_dm; }

protected:

	casadi::DM x0_dm;
	casadi::DM ubx_dm;
	casadi::DM lbx_dm;

};
typedef VmC VM;

/*
 * OCP formulation based on terminology of Betts, 2010 Ch4, p 123
 */
class OcpC {

public:

	OcpC( float tf, std::string name_="empty" ) : name(name_) { t = VmC("t",casadi::DM(1), casadi::DM(tf)); }
	virtual ~OcpC( void ) {};

	virtual casadi::MX l(void) = 0;                     // legendre cost term
    virtual casadi::MX m(void){ return casadi::MX(); } ;// mayer cost term

	virtual casadi::MX f(void) = 0;                     // dynamic state equality constraints
	virtual casadi::MX g(void){ return casadi::MX(); }  // algebraic equality constraints
	virtual casadi::MX h(void){ return casadi::MX(); }  // algebraic inequality constraints

	VM t,y,u;                                           // time, states, control
    std::string name;
};

/*
 * Application class
 */
class CstrC: public OcpC {
public:

	CstrC( float );

	casadi::MX l(void);
	casadi::MX m(void);
	casadi::MX f(void);

};

/*
 * Application target to by transcribed
 */
typedef CstrC AppOcpC;


#endif /* INCLUDE_OCP_H_ */
