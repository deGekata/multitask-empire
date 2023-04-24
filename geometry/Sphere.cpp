#include "geometry/Sphere.hpp"
#include "geometry/Geometry.hpp"

namespace Geometry {

bool Sphere3f::contains(const Point3f& pt) const {
    return (pt - center_).len() < r_ - EPS;
}

bool Sphere3f::containsB(const Point3f& pt) const {
    return (pt - center_).len() < r_ + EPS;
}

bool Sphere3f::isOnSurface(const Point3f& pt) const {
    return ((pt - center_).len() - r_) < EPS;
}

bool operator==(const Sphere3f& lhs, const Sphere3f& rhs) {
    return lhs.center() == rhs.center() && isZero(lhs.r() - rhs.r());
}

}  // namespace Geometry
