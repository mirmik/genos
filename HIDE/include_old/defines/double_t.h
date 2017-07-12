#ifndef DOUBLE_T_DEF_H_
#define DOUBLE_T_DEF_H_

#include "bits/float.h"

#if FLT_EVAL_METHOD == 0
typedef double double_t ; 
#elif FLT_EVAL_METHOD == 1
typedef double double_t; 
#elif FLT_EVAL_METHOD == 2
typedef long double double_t; 
#endif


#endif