#include "graphics/color.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>

namespace igraphicslib {

Color& Color::operator+=(const Color& rhs) {
    asRGBA_.r_ = (static_cast<uint32_t>(asRGBA_.r_) + rhs.asRGBA_.r_ > 255) ? 255 : (asRGBA_.r_ + rhs.asRGBA_.r_);
    asRGBA_.g_ = (static_cast<uint32_t>(asRGBA_.g_) + rhs.asRGBA_.g_ > 255) ? 255 : (asRGBA_.g_ + rhs.asRGBA_.g_);
    asRGBA_.b_ = (static_cast<uint32_t>(asRGBA_.b_) + rhs.asRGBA_.b_ > 255) ? 255 : (asRGBA_.b_ + rhs.asRGBA_.b_);
    asRGBA_.a_ = (static_cast<uint32_t>(asRGBA_.a_) + rhs.asRGBA_.a_ > 255) ? 255 : (asRGBA_.a_ + rhs.asRGBA_.a_);
    return *this;
}

Color& Color::operator&=(const Color& rhs) {
    asRGBA_.r_ = static_cast<uint8_t>((asRGBA_.r_ * static_cast<uint32_t>(rhs.asRGBA_.r_) / 255));
    asRGBA_.g_ = static_cast<uint8_t>((asRGBA_.g_ * static_cast<uint32_t>(rhs.asRGBA_.g_) / 255));
    asRGBA_.b_ = static_cast<uint8_t>((asRGBA_.b_ * static_cast<uint32_t>(rhs.asRGBA_.b_) / 255));
    asRGBA_.a_ = static_cast<uint8_t>((asRGBA_.a_ * static_cast<uint32_t>(rhs.asRGBA_.a_) / 255));
    return *this;
}

Color& Color::operator|=(const Color& rhs) {
    asRGBA_.r_ = std::max(asRGBA_.r_, rhs.asRGBA_.r_);
    asRGBA_.g_ = std::max(asRGBA_.g_, rhs.asRGBA_.g_);
    asRGBA_.b_ = std::max(asRGBA_.b_, rhs.asRGBA_.b_);
    asRGBA_.a_ = std::max(asRGBA_.a_, rhs.asRGBA_.a_);
    return *this;
}

Color operator*(const Color& lhs, double rhs) {
    return Color{
        static_cast<uint8_t>(std::min(255., (lhs.R() * rhs))),
        static_cast<uint8_t>(std::min(255., (lhs.G() * rhs))),
        static_cast<uint8_t>(std::min(255., (lhs.B() * rhs))),
        lhs.A(),
    };
}

Color operator*(double lhs, const Color& rhs) {
    return rhs * lhs;
}

Color operator+(const Color& lhs, const Color& rhs) {
    Color res = lhs;
    return res += rhs;
}

Color GammaCorrect(Color col, double gamma) {
    col.R(static_cast<uint8_t>(255. * std::pow(col.R() / 255., 1 / gamma)));
    col.G(static_cast<uint8_t>(255. * std::pow(col.G() / 255., 1 / gamma)));
    col.B(static_cast<uint8_t>(255. * std::pow(col.B() / 255., 1 / gamma)));
    return col;
}

// void Color::setHSV(uint32_t h, uint8_t s, uint8_t v, uint8_t alpha)

uint32_t Color::H() const {
    int32_t r = R();
    int32_t g = G();
    int32_t b = B();

    int32_t max = std::max(std::max(r, g), b);
    int32_t min = std::min(std::min(r, g), b);
    if (max == min) {
        return 0;
    }

    if (max == r) {
        return static_cast<uint32_t>(60 * (g - b) / (max - min) + ((g < b) ? 360 : 0));
    }
    return static_cast<uint32_t>(60 * (max == g ? b - r : r - g) / (max - min) + (max == g ? 120 : 240));
}

uint8_t Color::S() const {
    uint32_t max = std::max(std::max(asRGBA_.r_, asRGBA_.g_), asRGBA_.b_);
    uint32_t min = std::min(std::min(asRGBA_.r_, asRGBA_.g_), asRGBA_.b_);
    return max ? static_cast<uint8_t>(100u * (max - min) / max) : 0;
}

uint8_t Color::V() const {
    return std::max(std::max(asRGBA_.r_, asRGBA_.g_), asRGBA_.b_) * 100u / 255;
}

void Color::H(uint32_t h) {
    SetHSV(h, S(), V(), A());
}

void Color::S(uint8_t s) {
    SetHSV(H(), s, V(), A());
}

void Color::V(uint8_t v) {
    SetHSV(H(), S(), v, A());
}

}  // namespace igraphicslib
