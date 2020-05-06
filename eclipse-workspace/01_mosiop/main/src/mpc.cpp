/*
 * mpc.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../inc/mpc.h"

casadi::DMDict MpcC::solve(casadi::DMDict& arg, bool closed_loop )
{
	casadi::DMDict ret;

	if( closed_loop ) {

		std::map<std::string, casadi::DMVector> cl;

		for( int k =0; k< ceil(T/h); k++) {

			casadi::DMDict nlp_res = nlp.solve( arg );

			cl["t"].push_back( k*h + nlp_res["t0"] );
			cl["x"].push_back( nlp_res["x0"] );
			cl["u"].push_back( nlp_res["u0"] );

			arg["x0"] = nlp_res["x1"](casadi::Slice(),casadi::Slice(0,1,1));
		}

		ret = {{"t",horzcat(cl["t"])},{"x",horzcat(cl["x"])},{"u",horzcat(cl["u"])}};

	}else {

		casadi::DMDict nlp_res = nlp.solve(arg);

		ret = {{"t",nlp_res["t"]},{"x",nlp_res["x"]},{"u",nlp_res["u"]}};
	}

	return ret;

}
