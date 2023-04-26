#ifndef GEOMETRY_FLOAT_HPP
#define GEOMETRY_FLOAT_HPP

#include <cmath>

#ifndef GEOMETRY_EPSILON
#define GEOMETRY_EPSILON 1e-8
#endif

namespace geometry {
const double kEPS = GEOMETRY_EPSILON;

inline bool IsZero(int x) {
    return x == 0;
}

inline bool IsZero(double x) {
    return fabs(x) < kEPS;
}
}  // namespace Geometry

#endif /* GEOMETRY_FLOAT_HPP */
