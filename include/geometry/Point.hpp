#ifndef GEOMETRY_POINT_HPP
#define GEOMETRY_POINT_HPP
#include <cassert>
#include <cstdint>
#include <cmath>

namespace geometry {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wsign-conversion"


/**
 * @brief The Point2 class
 */
template <typename NumT>
struct Point2 {
    NumT x, y;

    /**
     * @brief Point2 - point on surface
     * @param _x - x coordinate
     * @param _y - y coordinate
     */
    Point2(NumT x_new, NumT y_new) : x(x_new), y(y_new) {
    }

    /**
     * @brief Point2 default constructor (0, 0)
     */
    Point2() : Point2(0, 0) {
    }

    /**
     * @brief Point2 constructor from point of other type
     * @param point ------------------^
     */
    template <typename T>
    Point2(const Point2<T>& point) : x(point.x), y(point.y) { //NOLINT
    }

    std::strong_ordering operator<=>(const Point2& oth) const {
        if ((x <=> oth.x) != std::strong_ordering::equal) {
            return x <=> oth.x;
        }
        return y <=> oth.y;
    }

    bool operator==(const Point2&) const = default;
    bool operator!=(const Point2&) const = default;
    bool operator<=(const Point2&) const = default;
    bool operator>=(const Point2&) const = default;
    bool operator>(const Point2&) const = default;
    bool operator<(const Point2&) const = default;
};

/**
 * @brief The Point3 class
 */
template <typename NumT>
struct Point3 {
    NumT x, y, z;

    /**
     * @brief Point3
     * @param _x - x coordiante
     * @param _y - y coordiante
     * @param _z - z coordiante
     */
    Point3(NumT x_new, NumT y_new, NumT z_new) : x(x_new), y(y_new), z(z_new) {
    }

    /**
     * @brief Point3 default constructor (0,0,0)
     */
    Point3() : Point3(0, 0, 0) {
    }

    /**
     * @brief Point3 constructor from Point2
     * @param pt - Point2 point
     */
    explicit Point3(const Point2<NumT>& pt, NumT z_new = 0) : x(pt.x), y(pt.y), z(z_new) {
    }

    /**
     * @brief Point3 constructor from point of other type
     * @param pt ---------------------^
     */
    template <typename T>
    Point3(const Point3<T>& pt) : x(pt.x), y(pt.y), z(pt.z) { //NOLINT
    }
};

using Point2f = Point2<double>;
using Point2u = Point2<uint32_t>;
using Point2i = Point2<int32_t>;

using Point3f = Point3<double>;
using Point3u = Point3<uint32_t>;
using Point3i = Point3<int32_t>;

template <typename NumT>
bool operator==(const Point2<NumT>& lhs, const Point2<NumT>& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename NumT>
bool operator==(const Point3<NumT>& lhs, const Point3<NumT>& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename NumT>
bool operator!=(const Point2<NumT>& lhs, const Point2<NumT>& rhs) {
    return !(lhs == rhs);
}

template <typename NumT>
bool operator!=(const Point3<NumT>& lhs, const Point3<NumT>& rhs) {
    return !(lhs == rhs);
}

#pragma clang diagnostic pop

}  // namespace Geometry
#endif


