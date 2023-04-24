#ifndef GEOMETRY_SPHERE_HPP
#define GEOMETRY_SPHERE_HPP
#include <geometry/Point.hpp>
#include <geometry/GeomVector.hpp>

namespace Geometry {

class Sphere3f {
    Point3f center_;
    double r_;

public:
    Sphere3f(const Point3f& pt, double r) : center_(pt), r_(r) {
    }

    Sphere3f(const Point3f& pt, const Vector3f& v) : center_(pt), r_(v.len()) {
    }

    bool contains(const Point3f& pt) const;
    bool containsB(const Point3f& pt) const;
    bool isOnSurface(const Point3f& pt) const;

    const Point3f& center() const {
        return center_;
    }
    double r() const {
        return r_;
    }

    void setCenter(const Point3f& c) {
        center_ = c;
    }
    void setRadius(double radius) {
        r_ = radius;
    }
};

bool operator==(const Sphere3f& lhs, const Sphere3f& rhs);
}  // namespace Geometry

#endif /* GEOMETRY_SPHERE_HPP */
