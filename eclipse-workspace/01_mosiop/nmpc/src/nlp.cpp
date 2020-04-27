/*
 * nlp.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: johannes
 */

#include "../inc/nlp.h"

/*
 * NLP formulation class functions
 */

NlpC::NlpC(  float tf_, int N_ ):ocp(tf), tf(tf_), N(N_), dt(tf_/N)
{

};

