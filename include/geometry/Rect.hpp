#ifndef GEOMETRY_RECT_HPP
#define GEOMETRY_RECT_HPP
#include "geometry/Point.hpp"

namespace geometry {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wsign-conversion"
template <typename NumT>
struct Rect2 {
    
    NumT x, y, w, h;

    constexpr Rect2(NumT x_new, NumT y_new, NumT w_new, NumT h_new) : x(x_new), y(y_new), w(w_new), h(h_new) {
    }
    constexpr Rect2() : Rect2(0, 0, 0, 0) {
    }

    template <typename T>
    
    explicit Rect2(const Rect2<T>& oth) : x(oth.x), y(oth.y), w(oth.w), h(oth.h) {
    }

    Point2<NumT> GetCornerLL() const {
        return {x, y};
    }
    Point2<NumT> GetCornerGL() const {
        return {x + w - 1, y};
    }
    Point2<NumT> GetCornerLG() const {
        return {x, y + h - 1};
    }
    Point2<NumT> GetCornerGG() const {
        return {x + w - 1, y + h - 1};
    }
};

using Rect2u = Rect2<unsigned>;

#pragma clang diagnostic pop
}  // namespace Geometry

#endif /* GEOMETRY_RECT_HPP */
