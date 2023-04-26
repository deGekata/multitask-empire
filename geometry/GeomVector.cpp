#include <geometry/geom_vector.hpp>

#include <geometry/point.hpp>
#include <geometry/float.hpp>

namespace geometry {
Vector2f Rotate(const Vector2f& v, double a) {
    return {
        v.x * cos(a) + v.y * sin(a),
        -v.x * sin(a) + v.y * cos(a),
    };
}

Vector3f RotateZ(const Vector3f& v, double a) {
    return {v.x * cos(a) + v.y * sin(a), -v.x * sin(a) + v.y * cos(a), v.z};
}

Vector2f Normalize(Vector2f v) {
    if (v * v < kEPS) {
        v.x = v.y = 0;
        return v;
    }
    return v /= v.Len();
}

Vector3f Normalize(Vector3f v) {
    if (v * v < kEPS) {
        v.x = v.y = v.z = 0;
        return v;
    }
    return v /= v.Len();
}

bool operator==(const Vector2f& lhs, const Vector2f& rhs) {
    return IsZero(lhs.x - rhs.x) && IsZero(lhs.y - rhs.y);
}
bool operator==(const Vector3f& lhs, const Vector3f& rhs) {
    return IsZero(lhs.x - rhs.x) && IsZero(lhs.y - rhs.y) && IsZero(lhs.z - rhs.z);
}

template <>
bool operator==(const Point2f& lhs, const Point2f& rhs) {
    return IsZero(lhs.x - rhs.x) && IsZero(lhs.y - rhs.y);
}

template <>
bool operator==(const Point3f& lhs, const Point3f& rhs) {
    return IsZero(lhs.x - rhs.x) && IsZero(lhs.y - rhs.y) && IsZero(lhs.z - rhs.z);
}

}  // namespace Geometry
