#include "geometry/Line.hpp"
#include "geometry/Geometry.hpp"

namespace geometry {

Line3f::Line3f(const Point3f& p1, const Point3f& p2) : Line3f(p1, p2 - p1) {
    assert(!(p1 == p2));
}

bool Line3f::Contains(const Point3f& pt) const {
    return (point_ - pt) || dir_;
}

bool IsParallel(const Line3f& lhs, const Line3f& rhs) {
    return IsParallel(lhs.Dir(), rhs.Dir());
}

bool operator||(const Line3f& lhs, const Line3f& rhs) {
    return IsParallel(lhs, rhs);
}

bool operator==(const Line3f& lhs, const Line3f& rhs) {
    return (lhs.Dir() || rhs.Dir()) && lhs.Contains(rhs.Point());
}

bool operator==(const Ray3f& lhs, const Ray3f& rhs) {
    return IsColling(lhs.Dir(), rhs.Dir()) && (lhs.Point() == rhs.Point());
}

}  // namespace Geometry
