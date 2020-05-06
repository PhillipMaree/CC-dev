/*
 * solver.h
 *
 *  Created on: Apr 23, 2020
 *      Author: johannes
 */

#ifndef INC_MPC_H_
#define INC_MPC_H_

#include <string>
#include "matplotlibcpp.h"
#include "../../solver/inc/nlp.h"

class MpcC {
public:

	MpcC( float h_, int N, int T_, int K=3) : h(h_), T(T_), nlp( h_, N, K ){ };
	~MpcC( void){};

	casadi::DMDict solve(casadi::DMDict&, bool closed_loop=true );

protected:

	NlpC nlp;
	const int T;
	const float h;

};

class FigC {
public:
	FigC( void ) {}
	~FigC( void ) {}

	typedef std::vector< std::vector< double >> vv;

	void title( std::string s ) {
		matplotlibcpp::title( s );
	}

	void show( void ) {
		matplotlibcpp::show();
	}

	void plot( casadi::DM& t, casadi::DM& x, std::string linestyle="b-" ) {
		if( t.size1() > t.size2() )
			t = t.T();
		if( x.size1() > x.size2() )
			x = x.T();

		vv vvt;
		for( int i=0; i<t.size1(); i++){
			std::vector< double > vt;
			for( int j=0; j<t.size2(); j++){
				vt.push_back( t(i,j).operator double()  );
			}
			vvt.push_back( vt );
		}

		vv vvy;
		for( int i=0; i<x.size1(); i++){
			std::vector< double > vx;
			for( int j=0; j<x.size2(); j++){
				vx.push_back(  x(i,j).operator double()  );
			}
			vvy.push_back( vx );
		}

		int y_size = x.size1();

		if( y_size==1  ) {

			matplotlibcpp::plot(vvt[0], vvy[0]);

		} else {

			int subplot_cols = ceil(y_size/2);
			int subplot_rows = ceil((y_size - subplot_cols)/subplot_cols+1);

			for( int col=0; col<subplot_cols; col++ ) {

				for( int row=0; col<subplot_rows; row++ ) {

					if( row+col==y_size )
						return;

					matplotlibcpp::subplot( subplot_cols, subplot_rows, row+col+1 );

					if( t.size1() ==  y_size )
						matplotlibcpp::plot( vvt[row+col], vvy[row+col] );
					else
						matplotlibcpp::plot( vvt[0], vvy[row+col] );

				}
			}
		}
	}

	void plot( casadi::DM& t, casadi::DM& x, casadi::DM& u, std::string linestyle="b-" ) {

		if( t.size1() > t.size2() )
			t = t.T();
		if( x.size1() > x.size2() )
			x = x.T();
		if( u.size1() > u.size2() )
			u = u.T();

		vv vvt;
		for( int i=0; i<t.size1(); i++){
			std::vector< double > vt;
			for( int j=0; j<t.size2(); j++){
				vt.push_back( t(i,j).operator double()  );
			}
			vvt.push_back( vt );
		}

		vv vvy;
		for( int i=0; i<x.size1(); i++){
			std::vector< double > vx;
			for( int j=0; j<x.size2(); j++){
				vx.push_back(  x(i,j).operator double()  );
			}
			vvy.push_back( vx );
		}
		for( int i=0; i<u.size1(); i++){
			std::vector< double > vu;
			for( int j=0; j<u.size2(); j++){
				vu.push_back(  u(i,j).operator double()  );
			}
			vvy.push_back( vu );
		}

		int y_size = x.size1()+u.size1();

		if( y_size==1 ) {

			matplotlibcpp::plot(vvt[0], vvy[0]);

		} else {

			int subplot_cols = ceil(y_size/2);
			int subplot_rows = ceil((y_size - subplot_cols)/subplot_cols+1);

			for( int col=0; col<subplot_cols; col++ ) {

				for( int row=0; col<subplot_rows; row++ ) {

					if( row+col==y_size )
						return;

					matplotlibcpp::subplot( subplot_cols, subplot_rows, row+col+1 );

					if( t.size1() ==  y_size )
						matplotlibcpp::plot( vvt[row+col], vvy[row+col] );
					else
						matplotlibcpp::plot( vvt[0], vvy[row+col] );
				}
			}
		}
	}
};



#endif /* INC_MPC_H_ */
