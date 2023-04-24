#ifndef GEOMETRY_RECT_HPP
#define GEOMETRY_RECT_HPP
#include "geometry/Point.hpp"

namespace Geometry {

template <typename num_t>
struct Rect2 {
    num_t x, y, w, h;

    constexpr Rect2(num_t x_, num_t y_, num_t w_, num_t h_) : x(x_), y(y_), w(w_), h(h_) {
    }
    constexpr Rect2() : Rect2(0, 0, 0, 0) {
    }

    template <typename T>
    explicit Rect2(const Rect2<T>& oth) : x(oth.x), y(oth.y), w(oth.w), h(oth.h) {
    }

    Point2<num_t> getCornerLL() const {
        return {x, y};
    }
    Point2<num_t> getCornerGL() const {
        return {x + w - 1, y};
    }
    Point2<num_t> getCornerLG() const {
        return {x, y + h - 1};
    }
    Point2<num_t> getCornerGG() const {
        return {x + w - 1, y + h - 1};
    }
};

using Rect2u = Rect2<unsigned>;

}  // namespace Geometry

#endif /* GEOMETRY_RECT_HPP */
