//
// Created by EDZ on 2019/12/24.
//

#ifndef BALLSBATTLE_BBMATH_H
#define BALLSBATTLE_BBMATH_H

#include "math.h"

#define BB_M_PI       3.14159
#define BB_MATH_TOLERANCE              2e-37f

inline int abs_int(int  x){
    return static_cast<int>(fabs(static_cast<double>(x)));
}

#endif //BALLSBATTLE_BBMATH_H
