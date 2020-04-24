/*
 * math.h
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */

#ifndef INCLUDE_MATH_H_
#define INCLUDE_MATH_H_

#include <initializer_list>
#include <vector>

template<typename T>
class MatrixC : public std::vector<std::vector<T>> {
public:

	MatrixC<T>( void ){}
	MatrixC<T>(int rows, int cols){
		this->resize(rows);
		for ( int i = 0 ; i < rows ; i++ )
			this->data()[i].resize(cols);
	}

	int rows( void ){ return this->size(); }
	int cols( void ){ return this->data()[0].size(); }
};

template<typename T>
class VectorC : public std::vector<T> {
public:

	VectorC<T>( void ){}
	VectorC<T>( std::initializer_list<T> val) {
		this->assign( val );
	}
	VectorC<T>(int len){
		this->resize(len);
	}

	int len( void ){ return this->size(); }
	void set( std::initializer_list<T> val ){
		this->clear();
		this->assign( val );
	}
};

#endif /* INCLUDE_MATH_H_ */
