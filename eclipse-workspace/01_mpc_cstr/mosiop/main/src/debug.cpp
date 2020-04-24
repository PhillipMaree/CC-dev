/*
 * main.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: johannes
 */
#include "debug.h"

#ifdef DEBUG_ENABLED

void DEBUG( const std::string str  )
{
	printf("%s\n", str.c_str());
}



#endif



