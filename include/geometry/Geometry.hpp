#ifndef GEOMETRY_GEOMETRY_HPP
#define GEOMETRY_GEOMETRY_HPP

#include "geometry/GeomVector.hpp"
#include <geometry/Point.hpp>
#include <geometry/Rect.hpp>
#include <geometry/Line.hpp>
#include <geometry/Sphere.hpp>
#include <geometry/Intersects.hpp>
#include <geometry/Operations.hpp>
#include <random>

namespace geometry {

inline double RandomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double RandomDouble(double min, double max) {
    return min + (max - min) * RandomDouble();
}

inline Vector3f RandomInSphere() {
    Vector3f v{};
    do {
        v = {
            RandomDouble(-1, 1),
            RandomDouble(-1, 1),
            RandomDouble(-1, 1),
        };

    } while (v.Len2() > 1);
    return Normalize(v);
}
}  // namespace Geometry

#endif /* GEOMETRY_GEOMETRY_HPP */
