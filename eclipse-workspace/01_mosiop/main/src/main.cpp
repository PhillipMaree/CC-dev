/*
 * main.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */
#include "../../nmpc/inc/mpc.h"




typedef std::map<std::string, casadi::DMVector> DMVDict;
typedef std::map<std::string, casadi::DMVector>::iterator itr;

casadi::DM get( DMVDict d, std::string key ){
	casadi::DMVector v;

	itr it = d.find( key );
	if( it != d.end() ) {
		for(int i=0; i< it->second.size(); i++) {
			casadi::DM di;
			di = it->second[i];
			for(int j=0;j< (di.size1() < di.size2() ? di.size2() : di.size1() );j++)
				v.push_back(di(j));
		}
	}
	return casadi::DM(v);
}

int main( void )
{

	//MpcC mpc(1,10);
	//casadi::DMVector t;

	//casadi::DM a({1,2,3});

	//t.push_back(a);
	//t.push_back(a);
	//casadi::DM t_ =casadi::DM(t);
	//DEBUG( t_ , "t");

	DMVDict d;

	//d["t"] = std::vector<casadi::DM>({1});
	d["t"].push_back(std::vector<casadi::DM>({2,3}));
	d["t"].push_back(std::vector<casadi::DM>({4,5,6}));


    DEBUG( get(d,"t") , "t");

    DictC<casadi::DM> test;

    test.append("t",casadi::DM({1,2,3}));
    test.append("t",casadi::DM({-3}));
    test.append("t",casadi::DM({-3,3,4,5}));

    DEBUG( test.eval("t").T() , "t");

    DictC<casadi::DM> test2;

    test2["u"].append(casadi::DM({1,2,3}));
    test2["u"].append(casadi::DM({1}));

    test2["p"].append(casadi::DM({3,4,5}));
    test2["p"].append(casadi::DM({1,2,3}));


    DEBUG( test2["u"].eval() , "u");
    DEBUG( test2["p"].eval() , "p");

	return 0;
}





