#ifndef GEOMETRY_OPERATIONS_HPP
#define GEOMETRY_OPERATIONS_HPP
#include <geometry/float.hpp>
#include <geometry/point.hpp>
#include <geometry/rect.hpp>
#include <geometry/geom_vector.hpp>

namespace geometry {

template <typename NumT>
const Vector2<NumT>& AsVector(const Point2<NumT>& pt) {
    return reinterpret_cast<const Vector2<NumT>&>(pt);
}

template <typename NumT>
const Vector3<NumT>& AsVector(const Point3<NumT>& pt) {
    return reinterpret_cast<const Vector3<NumT>&>(pt);
}

template <typename NumT>
const Point2<NumT>& AsPoint(const Vector2<NumT>& pt) {
    return reinterpret_cast<const Point2<NumT>&>(pt);
}

template <typename NumT>
const Point3<NumT>& AsPoint(const Vector3<NumT>& pt) {
    return reinterpret_cast<const Point3<NumT>&>(pt);
}

template <typename NumT>
Point2<NumT> operator+=(Point2<NumT>& pt, const Vector2<NumT>& v) {
    pt.x += v.x;
    pt.y += v.y;
    return pt;
}

template <typename NumT>
Point2<NumT> operator-=(Point2<NumT>& pt, const Vector2<NumT>& v) {
    pt.x -= v.x;
    pt.y -= v.y;
    return pt;
}

template <typename NumT>
Point3<NumT> operator+=(Point3<NumT>& pt, const Vector3<NumT>& v) {
    pt.x += v.x;
    pt.y += v.y;
    pt.z += v.z;
    return pt;
}

template <typename NumT>
Point3<NumT> operator-=(Point3<NumT>& pt, const Vector3<NumT>& v) {
    pt.x -= v.x;
    pt.y -= v.y;
    pt.z -= v.z;
    return pt;
}

template <typename NumT>
Vector2<NumT> operator-(const Point2<NumT>& pt1, const Point2<NumT>& pt2) {
    return {pt1.x - pt2.x, pt1.y - pt2.y};
}

template <typename NumT>
Vector3<NumT> operator-(const Point3<NumT>& pt1, const Point3<NumT>& pt2) {
    return {pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z};
}

template <typename NumT>
double RadiusLen(const Point2<NumT>& pt) {
    return sqrt(pt.x * pt.x + pt.y * pt.y);
}

template <typename NumT>
double RadiusLen(const Point3<NumT>& pt) {
    return sqrt(pt.x * pt.x + pt.y * pt.y + pt.z * pt.z);
}

inline Point3f RotateZ(const Point3f& v, double a) {
    return {v.x * cos(a) + v.y * sin(a), -v.x * sin(a) + v.y * cos(a), v.z};
}

template <typename NumT>
inline bool Contains(const Rect2<NumT>& rect, const Point2<NumT>& pt) {
    return pt.x >= rect.x && pt.x <= rect.x + rect.w && pt.y >= rect.y && pt.y <= rect.y + rect.h;
}

inline bool Contains(const Rect2<unsigned>& rect, const Point2<int>& pt) {
    return Contains(Rect2<int>(rect), pt);
}

inline Vector3f Rotate(Vector3f, Vector3f, double) {
    return {};
}

}  // namespace Geometry

#endif /* GEOMETRY_OPERATIONS_HPP */
