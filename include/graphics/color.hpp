#ifndef ABSTRACTGL_COLOR_HPP
#define ABSTRACTGL_COLOR_HPP

#include <cstdint>
#include <cstdlib>

namespace igraphicslib {

class Color {

    struct ColorBytes {
        uint8_t a_, b_, g_, r_;
    };

    union {
        uint32_t color_;
        ColorBytes asRGBA_;
    };

public:
    // NOLINTBEGIN
    constexpr Color(uint32_t color = 0) : color_(color) {
    }
    // NOLINTEND

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : asRGBA_({a, b, g, r}) {
    }
    
    constexpr Color(const Color& oth) = default;
    constexpr Color& operator=(const Color& oth) = default;

    Color& operator=(uint32_t color) {
        color_ = color;
        return *this;
    }

    explicit operator uint32_t() const {
        return color_;
    }

    //NOLINTNEXTLINE
    uint32_t color() const {
        return color_;
    }

    uint8_t R() const {
        return asRGBA_.r_;
    }
    uint8_t G() const {
        return asRGBA_.g_;
    }
    uint8_t B() const {
        return asRGBA_.b_;
    }
    uint8_t A() const {
        return asRGBA_.a_;
    }

    void R(uint8_t r) {
        asRGBA_.r_ = r;
    }
    void G(uint8_t g) {
        asRGBA_.g_ = g;
    }
    void B(uint8_t b) {
        asRGBA_.b_ = b;
    }
    void A(uint8_t a) {
        asRGBA_.a_ = a;
    }

    constexpr void SetHSV(uint32_t h, uint8_t s, uint8_t v, uint8_t alpha = 255) {
        uint32_t H = h, S = s, V = v; //NOLINT
        uint32_t h_index = (H / 60) % 6;
        uint32_t v_min = (100 - S) * V / 100;
        uint32_t delta = (V - v_min) * (H % 60) / 60;
        uint32_t v_inc = v_min + delta;
        uint32_t v_dec = V - delta;

        V = 255 * V / 100;
        v_inc = 255 * v_inc / 100;
        v_dec = 255 * v_dec / 100;
        v_min = 255 * v_min / 100;

        switch (h_index) {
            case 0:
                asRGBA_.r_ = static_cast<uint8_t>(V);
                asRGBA_.g_ = static_cast<uint8_t>(v_inc);
                asRGBA_.b_ = static_cast<uint8_t>(v_min);
                break;
            case 1:
                asRGBA_.r_ = static_cast<uint8_t>(v_dec);
                asRGBA_.g_ = static_cast<uint8_t>(V);
                asRGBA_.b_ = static_cast<uint8_t>(v_min);
                break;
            case 2:
                asRGBA_.r_ = static_cast<uint8_t>(v_min);
                asRGBA_.g_ = static_cast<uint8_t>(V);
                asRGBA_.b_ = static_cast<uint8_t>(v_inc);
                break;
            case 3:
                asRGBA_.r_ = static_cast<uint8_t>(v_min);
                asRGBA_.g_ = static_cast<uint8_t>(v_dec);
                asRGBA_.b_ = static_cast<uint8_t>(V);
                break;
            case 4:
                asRGBA_.r_ = static_cast<uint8_t>(v_inc);
                asRGBA_.g_ = static_cast<uint8_t>(v_min);
                asRGBA_.b_ = static_cast<uint8_t>(V);
                break;
            case 5:
                asRGBA_.r_ = static_cast<uint8_t>(V);
                asRGBA_.g_ = static_cast<uint8_t>(v_min);
                asRGBA_.b_ = static_cast<uint8_t>(v_dec);
                break;
            default:
                abort();
        }
        asRGBA_.a_ = alpha;
    }

    static constexpr igraphicslib::Color FromHsv(uint32_t h, uint8_t s, uint8_t v, uint8_t alpha = 255) {
        igraphicslib::Color ret = {};
        ret.SetHSV(h, s, v, alpha);
        return ret;
    }

    uint32_t H() const;
    uint8_t S() const;
    uint8_t V() const;

    void H(uint32_t h);
    void S(uint8_t s);
    void V(uint8_t v);

    Color& operator+=(const Color& rhs);
    Color& operator*=(uint8_t k);
    Color& operator&=(const Color& rhs);
    Color& operator|=(const Color& rhs);
};

Color operator*(const Color& lhs, double rhs);
Color operator*(double lhs, const Color& rhs);

Color operator+(const Color& lhs, const Color& rhs);

Color GammaCorrect(Color col, double gamma);

namespace colors {
const Color kTransparent = 0;
const Color kWhite = 0xFFFFFFFF;
const Color kBlack = 0x000000FF;
const Color kRed = 0xFF0000FF;
const Color kGreen = 0x00FF00FF;
const Color kBlue = 0x0000FFFF;
const Color kGray = 0xA0A0A0FF;
const Color kDGray = 0x202020FF;
const Color kLGray = 0xD0D0D0FF;
const Color kYellow = 0xFFFF00FF;
const Color kMagenta = 0xFF00FFFF;
const Color kCyan = 0x00FFFFFF;
const Color kPink = 0xFF66C4FF;
}  // namespace Colors
}  // namespace igraphicslib

#endif /* ABSTRACTGL_COLOR_HPP */
