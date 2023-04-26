#include "geometry/Sphere.hpp"
#include "geometry/Geometry.hpp"

namespace geometry {

bool Sphere3f::Contains(const Point3f& pt) const {
    return (pt - center_).Len() < r_ - kEPS;
}

bool Sphere3f::ContainsB(const Point3f& pt) const {
    return (pt - center_).Len() < r_ + kEPS;
}

bool Sphere3f::IsOnSurface(const Point3f& pt) const {
    return ((pt - center_).Len() - r_) < kEPS;
}

bool operator==(const Sphere3f& lhs, const Sphere3f& rhs) {
    return lhs.Center() == rhs.Center() && IsZero(lhs.R() - rhs.R());
}

}  // namespace Geometry
