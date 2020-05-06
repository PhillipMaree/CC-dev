/*
 * mpc.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "matplotlibcpp.h"
#include "../inc/mpc.h"

namespace plt {

	typedef std::vector< std::vector< double >> vv;

	void title( std::string s ) {
		matplotlibcpp::title( s );
	}

	void show( void ) {
		matplotlibcpp::show();
	}

	void plot( casadi::DM& x, casadi::DM& y, std::string linestyle="b-" ) {

		if( x.size1() > x.size2() )
			x = x.T();
		if( y.size1() > y.size2() )
			y = y.T();

		vv vvx;
		for( int i=0; i<x.size1(); i++){
			std::vector< double > vx;
			for( int j=0; j<x.size2(); j++){
				vx.push_back( x(i,j).operator double()  );
			}
			vvx.push_back( vx );
		}

		vv vvy;
		for( int i=0; i<y.size1(); i++){
			std::vector< double > vy;
			for( int j=0; j<y.size2(); j++){
				vy.push_back(  y(i,j).operator double()  );
			}
			vvy.push_back( vy );
		}

		if( y.size1()==1  ) {

			matplotlibcpp::plot(vvx[0], vvy[0]);

		} else {

			int subplot_cols = ceil(y.size1()/2);
			int subplot_rows = ceil((y.size1() - subplot_cols)/subplot_cols+1);



			for( int col=0; col<subplot_cols; col++ ) {

				for( int row=0; col<subplot_rows; row++ ) {

					if( row+col==y.size1() )
						return;

					matplotlibcpp::subplot( subplot_cols, subplot_rows, row+col+1 );

					if( x.size1() ==  y.size1() )
						matplotlibcpp::plot( vvx[row+col], vvy[row+col] );
					else
						matplotlibcpp::plot( vvx[0], vvy[row+col] );

				}
			}
		}
	}

	void plot( casadi::DM& x, casadi::DM& y1, casadi::DM& y2, std::string linestyle="b-" ) {

			if( x.size1() > x.size2() )
				x = x.T();
			if( y1.size1() > y1.size2() )
				y1 = y1.T();
			if( y2.size1() > y2.size2() )
				y2 = y2.T();

			printf("tn %d yn %d un %d\n", (int)x.size2(), (int)y1.size2(), (int)y2.size2());

			vv vvx;
			for( int i=0; i<x.size1(); i++){
				std::vector< double > vx;
				for( int j=0; j<x.size2(); j++){
					vx.push_back( x(i,j).operator double()  );
				}
				vvx.push_back( vx );
			}

			vv vvy;
			for( int i=0; i<y1.size1(); i++){
				std::vector< double > vy;
				for( int j=0; j<y1.size2(); j++){
					vy.push_back(  y1(i,j).operator double()  );
				}
				vvy.push_back( vy );
			}
			for( int i=0; i<y2.size1(); i++){
				std::vector< double > vy;
				for( int j=0; j<y2.size2(); j++){
					vy.push_back(  y2(i,j).operator double()  );
				}
				vvy.push_back( vy );
			}

			int y_size = y1.size1()+y2.size1();

			if( y_size==1  ) {

				matplotlibcpp::plot(vvx[0], vvy[0]);

			} else {

				int subplot_cols = ceil(y_size/2);
				int subplot_rows = ceil((y_size - subplot_cols)/subplot_cols+1);



				for( int col=0; col<subplot_cols; col++ ) {

					for( int row=0; col<subplot_rows; row++ ) {

						if( row+col==y_size )
							return;

						printf("c=%d, r=%d, p=%d\n", subplot_cols, subplot_rows, row+col+1);

						matplotlibcpp::subplot( subplot_cols, subplot_rows, row+col+1 );

						if( x.size1() ==  y_size )
							matplotlibcpp::plot( vvx[row+col], vvy[row+col] );
						else
							matplotlibcpp::plot( vvx[0], vvy[row+col] );

					}
				}
			}
		}
}

void MpcC::solve(casadi::DMDict& arg, bool plot_ol, bool plot_cl)
{
	//Gnuplot gp;

	casadi::DMDict res;
	const int N = ceil(T/h);

	//for( int i =0; i< N; i++) {

		// solve open-loop OCP

	    res = nlp.solve(arg);

	    if( plot_ol ) {
		    plt::plot(res["t"],res["x"], res["u"] );
		    plt::show();
	    }

		DEBUG(res["u"],"u*");
		DEBUG(res["x"],"x*");
		DEBUG(res["t"],"t");
		DEBUG(res["x0"],"x0*");
		DEBUG(res["x1"],"x1*");
		DEBUG(res["u0"],"u0*");

	//}

}





