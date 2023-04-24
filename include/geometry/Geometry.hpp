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

namespace Geometry {

inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

inline Vector3f randomInSphere() {
    Vector3f v{};
    do {
        v = {
            randomDouble(-1, 1),
            randomDouble(-1, 1),
            randomDouble(-1, 1),
        };

    } while (v.len2() > 1);
    return normalize(v);
}
}  // namespace Geometry

#endif /* GEOMETRY_GEOMETRY_HPP */
