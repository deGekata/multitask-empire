#ifndef GEOMETRY_PLANE_HPP
#define GEOMETRY_PLANE_HPP
#include <geometry/geomVector.hpp>
#include <geometry/point.hpp>

namespace geometry {

class Plane3f {
    Vector3f normal_;
    double dist_;

    Plane3f(const Vector3f& normal, double dist) : normal_(Normalize(normal)), dist_(dist) {
    }

public:
    Plane3f(const Vector3f& normal, const Point3f& pt);

    bool Contains(const Point3f& pt) const;
    bool Contains(const Vector3f& v) const;

    double Dist2(const Point3f& pt) const;
    double Dist(const Point3f& pt) const;

    const Vector3f& Normal() const {
        return normal_;
    }
    double D() const {
        return dist_;
    }
};

bool operator==(const Plane3f& lhs, const Plane3f& rhs);

bool IsParallel(const Plane3f& lhs, const Plane3f& rhs);
bool IsParallel(const Plane3f& lhs, const Vector3f& rhs);
bool IsParallel(const Vector3f& lhs, const Plane3f& rhs);

bool operator||(const Plane3f& lhs, const Plane3f& rhs);
bool operator||(const Plane3f& lhs, const Vector3f& rhs);
bool operator||(const Vector3f& lhs, const Plane3f& rhs);

}  // namespace Geometry

#endif /* GEOMETRY_PLANE_HPP */
