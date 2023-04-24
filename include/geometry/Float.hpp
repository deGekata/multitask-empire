#ifndef GEOMETRY_FLOAT_HPP
#define GEOMETRY_FLOAT_HPP

#include <cmath>

#ifndef GEOMETRY_EPSILON
#define GEOMETRY_EPSILON 1e-8
#endif

namespace Geometry {
const double EPS = GEOMETRY_EPSILON;

inline bool isZero(int x) {
    return x == 0;
}

inline bool isZero(double x) {
    return fabs(x) < EPS;
}
}  // namespace Geometry

#endif /* GEOMETRY_FLOAT_HPP */
