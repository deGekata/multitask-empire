#ifndef GEOMETRY_SPHERE_HPP
#define GEOMETRY_SPHERE_HPP
#include <geometry/point.hpp>
#include <geometry/geom_vector.hpp>

namespace geometry {

class Sphere3f {
    Point3f center_;
    double r_;

public:
    Sphere3f(const Point3f& pt, double r) : center_(pt), r_(r) {
    }

    Sphere3f(const Point3f& pt, const Vector3f& v) : center_(pt), r_(v.Len()) {
    }

    bool Contains(const Point3f& pt) const;
    bool ContainsB(const Point3f& pt) const;
    bool IsOnSurface(const Point3f& pt) const;

    const Point3f& Center() const {
        return center_;
    }
    double R() const {
        return r_;
    }

    void SetCenter(const Point3f& c) {
        center_ = c;
    }
    void SetRadius(double radius) {
        r_ = radius;
    }
};

bool operator==(const Sphere3f& lhs, const Sphere3f& rhs);
}  // namespace geometry

#endif /* GEOMETRY_SPHERE_HPP */
