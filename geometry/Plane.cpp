#include "geometry/Plane.hpp"
#include "geometry/Operations.hpp"
namespace geometry {

Plane3f::Plane3f(const Vector3f& normal, const Point3f& pt)
    : normal_(Normalize(normal)), dist_(normal_ * (pt - Point3f{0, 0, 0})) {
    if (normal_ * (pt - Point3f{0, 0, 0}) < 0) {
        normal_ *= -1;
        dist_ *= -1;
    }
}

double Plane3f::Dist2(const Point3f& pt) const {
    return (normal_ * (pt - Point3f{0, 0, 0})) - dist_;
}

bool Plane3f::Contains(const Point3f& pt) const {
    return IsZero(Dist2(pt));
}

double Plane3f::Dist(const Point3f& pt) const {
    return std::fabs(Dist2(pt));
}

bool Plane3f::Contains(const Vector3f& v) const {
    return IsZero(normal_ * v);
}

bool operator==(const Plane3f& lhs, const Plane3f& rhs) {
    return (lhs || rhs) && IsZero(lhs.D() - rhs.D());
}

bool IsParallel(const Plane3f& lhs, const Plane3f& rhs) {
    return (lhs.Normal() ^ rhs.Normal()).Len() < kEPS;
}

bool IsParallel(const Plane3f& lhs, const Vector3f& rhs) {
    return IsZero(lhs.Normal() * rhs);
}

bool IsParallel(const Vector3f& lhs, const Plane3f& rhs) {
    return IsZero(lhs * rhs.Normal());
}

bool operator||(const Plane3f& lhs, const Plane3f& rhs) {
    return IsParallel(lhs, rhs);
}

bool operator||(const Plane3f& lhs, const Vector3f& rhs) {
    return IsParallel(lhs, rhs);
}

bool operator||(const Vector3f& lhs, const Plane3f& rhs) {
    return IsParallel(lhs, rhs);
}

}  // namespace Geometry
