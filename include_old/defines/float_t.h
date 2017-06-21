
#ifndef FLOAT_T_DEF_H_
#define FLOAT_T_DEF_H_

#include "bits/float.h"

#if FLT_EVAL_METHOD == 0
typedef float float_t;
#elif FLT_EVAL_METHOD == 1
typedef double float_t;
#elif FLT_EVAL_METHOD == 2
typedef long double float_t;
#endif


#endif